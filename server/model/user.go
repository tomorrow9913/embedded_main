package model

type User struct {
	Id uint        `gorm:"primaryKey"`
	Balance int
}