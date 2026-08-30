export type camera = {
	move: (x: number, y: number, z: number) -> (),
	rotate: (x: number, y: number, z: number) -> (),
}

export type input = {
	connectKeyPress: (() -> ()) -> (),
}

declare camera : camera
declare input : input