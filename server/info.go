package main

import (
	"github.com/gofiber/fiber/v2"
)

func createInfo(c *fiber.Ctx) error {
	return c.SendString("create info");
}

func readInfo(c *fiber.Ctx) error {
	return c.SendString("read info");
}

func updateInfo(c *fiber.Ctx) error {
	return c.SendString("update info");
}

func deleteInfo(c *fiber.Ctx) error {
	return c.SendString("delete info");
}