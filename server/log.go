package main

import (
	"server/models"
	"github.com/gofiber/fiber/v2"
)

func readLog(c *fiber.Ctx) error {
	var logs []models.Log

	if r := DB.Find(&logs); r.Error != nil {
		return r.Error
	}

	return c.JSON(fiber.Map{
		"logs": logs,
	})
}

func readLogByID(c *fiber.Ctx) error {
	var log models.Log

	if r := DB.First(&log, c.Params("id")); r.Error != nil {
		return r.Error
	}

	return c.JSON(log)
}

func readLogByUser(c *fiber.Ctx) error {
	var logs []models.Log

	if r := DB.Where("user_id=?", c.Params("id")).Find(&logs); r.Error != nil {
		return r.Error
	}

	return c.JSON(fiber.Map{
		"logs": logs,
	})
}