package main

import (
	"github.com/gofiber/fiber/v2"
)

func createLog(c *fiber.Ctx) error {
	return c.SendString("create log");
}

func readLog(c *fiber.Ctx) error {
	return c.SendString("read log");
}

func updateLog(c *fiber.Ctx) error {
	return c.SendString("update log");
}

func deleteLog(c *fiber.Ctx) error {
	return c.SendString("delete log");
}