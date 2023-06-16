package model

type Item struct {
	Id uint    `gorm:"primaryKey"`
	Price int
	TotalStock int
}