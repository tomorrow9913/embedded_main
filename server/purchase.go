package main

import (
	"github.com/gofiber/fiber/v2"
)

func createPurchase(c *fiber.Ctx) error {
	return c.SendString("create purchase")
}

func readPurchase(c *fiber.Ctx) error {
	return c.SendString("read purchase")
}

func updatePurchase(c *fiber.Ctx) error {
	return c.SendString("update purchase")
}

func deletePurchase(c *fiber.Ctx) error {
	return c.SendString("delete purchase")
}