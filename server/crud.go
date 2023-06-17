package main

import (
	"log"
	"github.com/gofiber/fiber/v2"
)

type Model struct {
	Id uint
}

func createData[T any](c *fiber.Ctx) error {
	// Get body
	var body T
	if err := c.BodyParser(&body); err != nil {
		return err
	}

	// Create data
	if r := DB.Create(&body); r.Error != nil {
		return r.Error
	}

	// Response
	log.Printf("create data %v", body)
	return c.SendString("create data")
}

func readAllData[T any](c *fiber.Ctx) error {
	// Read data from DB
	var data []T
	if r := DB.Find(&data); r.Error != nil {
		return r.Error
	}

	// Response
	return c.JSON(fiber.Map{
		"data": data,
	})
}

func readData[T any](c *fiber.Ctx) error {
	// Get id
	id := c.Params("id")

	// Read data from DB
	var data T
	if r := DB.First(&data, "id=?", id); r.Error != nil {
		return r.Error
	}

	// Response
	log.Printf("read data (id: %d)\n", id)
	return c.JSON(data)
}

func updateData[T any](c *fiber.Ctx) error {
	// Get body
	var body T
	if err := c.BodyParser(&body); err != nil {
		return err
	}

	// Update data from DB
	if r := DB.Model(&body).Updates(&body); r.Error != nil {
		return r.Error
	}

	// Response
	log.Printf("update data %v\n", body)
	return c.SendString("update data")
}


func updateAllData[T any](c *fiber.Ctx) error {
	// Get body
	var body T
	if err := c.BodyParser(&body); err != nil {
		return err
	}

	// Update data from DB
	if r := DB.Model(&body).Select("*").Updates(&body); r.Error != nil {
		return r.Error
	}

	// Response
	log.Printf("update data %v\n", body)
	return c.SendString("update data")
}

func deleteData[T any](c *fiber.Ctx) error {
	// Get id
	id := c.Params("id")

	// Delete data from DB
	var data T
	if r := DB.Delete(&data, "id=?", id); r.Error != nil {
		return r.Error
	}

	// Response
	log.Printf("delete item (id: %d)\n", id)
	return c.SendString("delete item")
}