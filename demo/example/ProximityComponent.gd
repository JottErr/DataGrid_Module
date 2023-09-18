class_name ProximityComponent
extends Node2D

@export var radius := 0.0
@export var registered := false
var last_registered_position := Vector2()
var last_registered_radius := 0.0

func _ready() -> void:
	add_to_group("ProximityComponents")
