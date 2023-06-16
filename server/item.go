package main

import (
	"fmt"
	"log"
	"strconv"
	"server/model"
	"github.com/gofiber/fiber/v2"
)

func createItem(c *fiber.Ctx) error {
	payload := new(model.Item)
	if err := c.BodyParser(payload); err != nil {
		return err
	}

	result := DB.Create(payload)
	if result.Error != nil {
		return result.Error
	}

	log.Printf("create item %v\n", payload)
	return c.SendString(fmt.Sprintf("create item %v", payload))
}

func readItem(c *fiber.Ctx) error {
	id, err := strconv.ParseUint(c.Params("id"), 10, 32)
	if err != nil {
		return err
	}

	var item model.Item
	result := DB.First(&item, id)
	if result.Error != nil {
		return result.Error
	}

	log.Printf("read item (%d)\n", id)
	return c.JSON(item)
}

func updateItem(c *fiber.Ctx) error {
	id, err := strconv.ParseUint(c.Params("id"), 10, 32)
	if err != nil {
		return err
	}

	payload := new(model.Item)
	if err := c.BodyParser(payload); err != nil {
		return err
	}

	var item model.Item
	result := DB.First(&item, id).Updates(payload)
	if result.Error != nil {
		return result.Error
	}

	log.Printf("update item %d: %v\n", id, payload)
	return c.SendString("update item")
}

func deleteItem(c * fiber.Ctx) error {
	id, err := strconv.ParseUint(c.Params("id"), 10, 32)
	if err != nil {
		return err
	}

	var item model.Item

	result := DB.First(&item, id).Delete(&item, id)
	if result.Error != nil {
		return result.Error
	}

	log.Printf("delete item (%d)\n", id)
	return c.SendString("delete item")
}