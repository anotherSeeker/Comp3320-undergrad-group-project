declare camera : {
	move: (x: number, y: number, z: number) -> (),
	rotate: (x: number, y: number, z: number) -> (),
}

declare input : {
	connectKeyPress: ((key : number) -> ()) -> (),
	connectKeyLifted: ((key : number) -> ()) -> (),
}

declare runservice : {
	connectPreRender: ((dt : number) -> ()) -> (),
}