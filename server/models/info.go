package models

type Info struct {
	Id string `gorm:"primaryKey"`
	Value string
}