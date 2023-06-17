package models

type Item struct {
	Id string    `gorm:"primaryKey"`
	Price int
	Count int
	Purchases []Purchase
}