val tweakForm : bool -> id -> id -> int -> xbody
val notifySuccess : id -> int -> xbody
val notifyMimeTypeError : id -> xbody
val notifyFileSizeError : id -> xbody

val idToString : id -> string
val stringToId : string -> id
