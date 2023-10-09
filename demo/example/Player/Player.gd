extends CharacterBody2D

var speed := 220

func _process(_delta: float) -> void:
	var dir := Input.get_vector("ui_left", "ui_right", "ui_up", "ui_down")
	velocity = speed * dir
	move_and_slide()
