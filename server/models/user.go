package models

type User struct {
	Id uint        `gorm:"primaryKey"`
	Balance int
	Logs []Log
}