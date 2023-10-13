extends CharacterBody2D


var speed := 250


func _process(_delta: float) -> void:
	var dir := Input.get_vector("move_left", "move_right", "move_up", "move_down")
	velocity = speed * dir
	move_and_slide()
