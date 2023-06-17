package models

import "time"

type Purchase struct {
	Id uint `gorm:"primaryKey"`
	Session uint
	CreatedAt time.Time
	Count int
	ItemID uint
}