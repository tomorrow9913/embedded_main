package main

import (
	"log"
	"server/model"
	"gorm.io/gorm"
	"gorm.io/driver/sqlite"
	"github.com/gofiber/fiber/v2"
)

var (
	DB *gorm.DB
)

func hello(c *fiber.Ctx) error {
	return c.SendString("Hello, World!")
}

func createApp() *fiber.App {
	// Create app
	app := fiber.New()

	// Hello
	app.Get("/hello", hello)

	// Item
	item := app.Group("/item")
	item.Post("/", createItem)
	item.Get("/:id", readItem)
	item.Patch("/:id", updateItem)
	item.Delete("/:id", deleteItem)

	// User
	user := app.Group("/user")
	user.Post("/", createUser)
	user.Get("/", readUser)
	user.Patch("/", updateUser)
	user.Delete("/", deleteUser)

	// Purchase
	purchase := app.Group("/purchase")
	purchase.Post("/", createPurchase)
	purchase.Get("/", readPurchase)
	purchase.Patch("/", updatePurchase)
	purchase.Delete("/", deletePurchase)

	// Log
	log := app.Group("/log")
	log.Post("/", createLog)
	log.Get("/", readLog)
	log.Patch("/", updateLog)
	log.Delete("/", deleteLog)

	// Info
	info := app.Group("/info")
	info.Post("/", createInfo)
	info.Get("/", readInfo)
	info.Patch("/", updateInfo)
	info.Delete("/", deleteInfo)

	return app
}

func setupDB() {
	db, err := gorm.Open(sqlite.Open("data.db"), &gorm.Config{})
	if err != nil {
		panic(err)
	}

	db.AutoMigrate(&model.Item{})

	DB = db
}

func main() {
	// Setup database
	setupDB()

	// Start server
	app := createApp()
	log.Fatal(app.Listen("localhost:3000"))
}