package main

import (
	"log"
	"fmt"
	"time"
	"strconv"
	"server/models"
	"github.com/gofiber/fiber/v2"
)

func createPurchaseSession(c *fiber.Ctx) error {
	// Get session
	session, err := Store.Get(c)
	if err != nil {
		return err
	}

	// Get timestamp
	now := time.Now()
	sessionID := now.UnixNano()

	// Set session id
	session.Set("purchase", sessionID)

	// Save session
	if err := session.Save(); err != nil {
		return err
	}

	// Response
	log.Printf("create purchase session (session: %d)\n", sessionID)
	return c.SendString("create purchase session")
}

func readPurchase(c *fiber.Ctx) error {
	// Get session
	session, err := Store.Get(c)
	if err != nil {
		return err
	}

	// Get session id
	sessionID := session.Get("purchase")
	if sessionID == nil {
		return c.Status(fiber.StatusForbidden).SendString("Session Error")
	}

	// Get data from DB
	var purchases []models.Purchase
	if r := DB.Where("session=?", sessionID).Find(&purchases); r.Error != nil {
		return r.Error
	}

	// Response
	return c.JSON(fiber.Map{
		"purchases": purchases,
	})
}

func readPurchaseByUser(c *fiber.Ctx) error {
	// Get id
	id := c.Params("id")

	// Get data from DB
	var purchases []models.Purchase
	if r := DB.Where("session=?", id).Find(&purchases); r.Error != nil {
		return r.Error
	}

	// Response
	return c.JSON(fiber.Map{
		"purchases": purchases,
	})
}

func deletePurchaseSession(c *fiber.Ctx) error {
	// Get session
	session, err := Store.Get(c)
	if err != nil {
		return err
	}

	// Check session
	sessionID := session.Get("purchase")
	if sessionID == nil {
		return c.Status(fiber.StatusForbidden).SendString("Session Error")
	}

	// Delete session
	session.Delete("purchase")

	// Delete purchases in DB
	if r := DB.Where("session=?", sessionID).Delete(&models.Purchase{}); r.Error != nil {
		return r.Error
	}

	// Save session
	if err := session.Save(); err != nil {
		return err
	}

	// Response
	log.Printf("delete purchase session (session: %d)\n", sessionID)
	return c.SendString("delete purchase session")
}

func addPurchaseItem(c *fiber.Ctx) error {
	// Get session
	session, err := Store.Get(c)
	if err != nil {
		return err
	}

	// Check session
	sessionID := session.Get("purchase")
	if sessionID == nil {
		return c.Status(fiber.StatusForbidden).SendString("Session Error")
	}

	// Get body
	var purchase models.Purchase
	if err := c.BodyParser(&purchase); err != nil {
		return err
	}

	// Set data
	purchase.Id = 0
	purchase.Session = fmt.Sprint(sessionID)

	// Check item id
	var item models.Item
	if r := DB.First(&item, purchase.ItemID); r.Error != nil {
		return r.Error
	}

	// Set item name
	purchase.ItemName = item.Name

	// Checking for items that already exist
	var prevPurchase models.Purchase
	if r := DB.Where(
			"session=? AND item_id=?",
			purchase.Session,
			purchase.ItemID,
		).First(&prevPurchase); r.Error != nil {

		// Create data in DB
		if r := DB.Create(&purchase); r.Error != nil {
			return r.Error
		}

	} else {
		prevPurchase.Count += purchase.Count

		if r := DB.Save(&prevPurchase); r.Error != nil {
			return r.Error
		}
	}

	// Response
	return c.SendString("create purchase item")
}

func removePurchaseItem(c *fiber.Ctx) error {
	// Get session
	session, err := Store.Get(c)
	if err != nil {
		return err
	}

	// Get session id
	sessionID := session.Get("purchase")
	if sessionID == nil {
		return c.Status(fiber.StatusForbidden).SendString("Session Error")
	}

	// Get body
	var body models.Item
	if err := c.BodyParser(&body); err != nil {
		return err
	}

	// Get data from DB
	var purchase models.Purchase
	if r := DB.Where(
			"session=? AND item_id=?",
			sessionID,
			body.Id,
		).First(&purchase); r.Error != nil {

		return r.Error
	}

	purchase.Count -= body.Count

	if (purchase.Count > 0) {
		if r := DB.Save(&purchase); r.Error != nil {
			return r.Error
		}

	} else {
		// Delete data from DB
		if r := DB.Delete(&models.Purchase{}, purchase.Id); r.Error != nil {
			return r.Error
		}
	}

	// Response
	log.Printf("delete purchase item (session: %d)\n", sessionID)
	return c.SendString("delete purchase item")
}

func signPurchase(c *fiber.Ctx) error {
	// Get session
	session, err := Store.Get(c)
	if err != nil {
		return err
	}

	// Check session
	sessionID := session.Get("purchase")
	if sessionID == nil {
		return c.Status(fiber.StatusForbidden).SendString("Session Error")
	}

	// Get user id
	userID, err := strconv.ParseUint(c.Params("user"), 10, 32)
	if err != nil {
		return err
	}

	// Get user data from DB
	var user models.User
	if r := DB.First(&user, userID); r.Error != nil {
		return r.Error
	}

	// Get purchase items
	var purchases []models.Purchase
	if r := DB.Where("session=?", sessionID).Find(&purchases); r.Error != nil {
		return r.Error
	}

	// Check total price
	totalPrice := 0

	for _, purchase := range purchases {
		// Get item
		var item models.Item
		if r := DB.First(&item, purchase.ItemID); r.Error != nil {
			return r.Error
		}

		if item.Count < purchase.Count {
			c.Status(fiber.StatusBadRequest)
			return c.SendString("Logic Error: Stock and purchase amounts don't match")
		}

		totalPrice += item.Price * purchase.Count
	}

	if totalPrice > user.Balance {
		c.Status(fiber.StatusBadRequest)
		return c.SendString("Logic Error: Insufficient user account balance")
	}

	user.Balance -= totalPrice
	if r := DB.Save(&user); r.Error != nil {
		return r.Error
	}

	// Set item count
	for _, purchase := range purchases {
		// Get item
		var item models.Item
		if r := DB.First(&item, purchase.ItemID); r.Error != nil {
			return r.Error
		}

		item.Count -= purchase.Count

		if r := DB.Save(&item); r.Error != nil {
			return r.Error
		}
	}

	// Get last balance
	var lastLog models.Log
	if r := DB.Last(&lastLog); r.Error != nil {
		lastLog.Balance = 0
	}

	// Create log in DB
	log := models.Log{
		Session: uint(sessionID.(int64)),
		UserID: uint(userID),
		Price: totalPrice,
		Balance: lastLog.Balance + totalPrice,
	}

	if r := DB.Create(&log); r.Error != nil {
		return r.Error
	}

	// Delete session
	session.Delete("purchase")
	if err := session.Save(); err != nil {
		return err
	}

	// Response
	return c.SendString("sign purchase")
}