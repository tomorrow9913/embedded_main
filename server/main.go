package main

import (
	"log"
	"flag"
	"gorm.io/gorm"
	"server/models"
	"gorm.io/driver/sqlite"
	"github.com/goccy/go-json"
	"github.com/gofiber/fiber/v2"
	"github.com/gofiber/fiber/v2/middleware/cors"
	"github.com/gofiber/fiber/v2/middleware/logger"
	"github.com/gofiber/fiber/v2/middleware/session"
	"github.com/gofiber/fiber/v2/middleware/basicauth"
)

var (
	DB *gorm.DB
	Store *session.Store
)

func hello(c *fiber.Ctx) error {
	return c.SendString("Hello, World!")
}

func setupDB() {
	db, err := gorm.Open(sqlite.Open("data.db"), &gorm.Config{})
	if err != nil {
		panic(err)
	}

	db.AutoMigrate(
		&models.Item{},
		&models.User{},
		&models.Purchase{},
		&models.Log{},
		&models.Info{},
	)

	DB = db
}

func createApp() *fiber.App {
	// Create app
	app := fiber.New(fiber.Config{
		JSONEncoder: json.Marshal,
		JSONDecoder: json.Unmarshal,
	})

	Store = session.New()

	app.Use(cors.New())
	app.Use(logger.New())

	auth := basicauth.New(basicauth.Config{
		Users: map[string]string{
			"admin": "admin",
		},
	})

	// Hello
	app.Get("/hello", hello)

	// Static
	app.Static("/", "./static")

	// Item
	item := app.Group("/item")
	item.Use(auth)
	item.Post("/", createData[models.Item])
	item.Get("/", readAllData[models.Item])
	item.Get("/:id", readData[models.Item])
	item.Patch("/", updateData[models.Item])
	item.Put("/", updateAllData[models.Item])
	item.Delete("/:id", deleteData[models.Item])

	// User
	user := app.Group("/user")
	item.Use(auth)
	user.Post("/", createData[models.User])
	user.Get("/:id", readData[models.User])
	user.Patch("/", updateData[models.User])
	user.Put("/", updateAllData[models.User])
	user.Delete("/:id", deleteData[models.User])

	// Purchase
	purchase := app.Group("/purchase")
	purchase.Post("/", createPurchaseSession)
	purchase.Get("/", readPurchase)
	purchase.Get("/:id", readPurchaseByUser)
	purchase.Delete("/", deletePurchaseSession)
	purchase.Post("/item", addPurchaseItem)
	purchase.Delete("/item/:id", removePurchaseItem)
	purchase.Get("/sign/:user", signPurchase)

	// Log
	log := app.Group("/log")
	item.Use(auth)
	log.Get("/", readLog)
	log.Get("/:id", readLogByID)
	log.Get("/user/:id", readLogByUser)

	// Info
	info := app.Group("/info")
	item.Use(auth)
	info.Post("/", createData[models.Info])
	info.Get("/:id", readData[models.Info])
	info.Patch("/", updateData[models.Info])
	info.Put("/", updateAllData[models.Info])
	info.Delete("/:id", deleteData[models.Info])

	return app
}

func main() {
	// Get flags
	var addr string
	flag.StringVar(&addr, "addr", "localhost:3000", "Server address and port")
	flag.Parse()

	// Setup database
	setupDB()

	// Start server
	app := createApp()
	log.Fatal(app.Listen(addr))
}