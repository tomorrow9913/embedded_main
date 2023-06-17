package main

import (
	"github.com/gofiber/fiber/v2"
)

func postSync(c *fiber.Ctx) error {
	// Get id
	id := c.Params("id")

	// Get session
	session, err := Store.Get(c)
	if err != nil {
		return err
	}

	// Save Session
	SyncData[id] = session.Get("purchase")

	// Response
	return c.SendString("save sync")
}

func getSync(c *fiber.Ctx) error {
	// Get id
	id := c.Params("id")

	// Get session
	session, err := Store.Get(c)
	if err != nil {
		return err
	}

	// Sync
	session.Set("purchase", SyncData[id])

	// Delete sync data
	SyncData[id] = nil

	// Save session
	if err := session.Save(); err != nil {
		return err
	}

	// Response
	return c.SendString("check sync")
}