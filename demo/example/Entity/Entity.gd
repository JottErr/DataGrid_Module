extends CharacterBody2D


var speed := 150
var target_position := Vector2()


func _physics_process(_delta: float) -> void:
	var direction := target_position.normalized()
	velocity = direction * speed
	move_and_slide()


func _on_thinky_part_found_target(target_pos) -> void:
	target_position = target_pos
