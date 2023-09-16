class_name ProximityComponent
extends Node2D

@export var radius := 0.0
@export var registered := false
@export var last_registered_position := Vector2()

func _ready() -> void:
	add_to_group("ProximityComponents")
