package models

import "time"

type Log struct {
	Id uint `gorm:"primaryKey"`
	CreatedAt time.Time
	Session uint
	UserID uint
	Price int
	Balance int
}