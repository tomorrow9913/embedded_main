package main

import (
	"github.com/gofiber/fiber/v2"
)

func createUser(c *fiber.Ctx) error {
	return c.SendString("create user");
}

func readUser(c *fiber.Ctx) error {
	return c.SendString("read user");
}

func updateUser(c *fiber.Ctx) error {
	return c.SendString("update user");
}

func deleteUser(c *fiber.Ctx) error {
	return c.SendString("delete user");
}