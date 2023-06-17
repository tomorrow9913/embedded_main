package models

type User struct {
	Id string        `gorm:"primaryKey"`
	Balance int
}