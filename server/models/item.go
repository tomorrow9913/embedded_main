package models

type Item struct {
	Id uint    `gorm:"primaryKey"`
	Price int
	Count int
	Purchases []Purchase
}