
/// this struct is used to keep track of the level 1 components (aka single pieces) a piece group is made of...
/// when a piece is constructed his sub components will be the union of all the level 1 components of his 4 sub components.
/// when 4 components are merge an error could be thrown if they have at least one level 1 component in common
struct PieceSubComponents{

}