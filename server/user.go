package main

import (
	"log"
	"strconv"
	"server/model"
	"github.com/gofiber/fiber/v2"
)

func createUser(c *fiber.Ctx) error {
	user := new(model.User)
	if err := c.BodyParser(user); err != nil {
		return err
	}
	
	result := DB.Create(&user)
	if result.Error != nil {
		return result.Error
	}

	log.Printf("create user %v\n", user)
	return c.SendString("create user")
}

func readUser(c *fiber.Ctx) error {
	id, err := strconv.ParseUint(c.Params("id"), 10, 32)
	if err != nil {
		return err
	}

	var user model.User
	result := DB.First(&user, id)
	if result.Error != nil {
		return result.Error
	}

	log.Printf("read user %v\n", user)
	return c.JSON(user)
}

func updateUser(c *fiber.Ctx) error {
	id, err := strconv.ParseUint(c.Params("id"), 10, 32)
	if err != nil {
		return err
	}

	payload := new(model.User)
	if err := c.BodyParser(payload); err != nil {
		return err
	}	

	var user model.User
	result := DB.First(&user, id).Updates(payload)
	if result.Error != nil {
		return result.Error
	}

	log.Printf("update user %v", payload)
	return c.SendString("update user")
}

func deleteUser(c *fiber.Ctx) error {
	id, err := strconv.ParseUint(c.Params("id"), 10, 32)
	if err != nil {
		return err
	}

	var user model.User
	result := DB.First(&user, id).Delete(&user, id)
	if result.Error != nil {
		return result.Error
	}

	log.Printf("delete user (%d)", id)
	return c.SendString("delete user")
}