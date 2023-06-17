package models

type Item struct {
	Id string    `gorm:"primaryKey"`
	Name string
	Price int
	Count int
}