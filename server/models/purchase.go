package models

import "time"

type Purchase struct {
	Id uint `gorm:"primaryKey"`
	Session string
	CreatedAt time.Time
	Count int
	ItemID string
	ItemName string
}