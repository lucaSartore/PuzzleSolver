// the 4 direction a piece can be compared on

use crate::piece_group::Direction::{DOWN, LEFT, RIGHT, UP};

#[derive(Clone,Copy)]
pub enum Direction {
    UP = 0,
    RIGHT = 1,
    DOWN = 2,
    LEFT = 3
}

impl std::ops::Neg for Direction{
    type Output = Direction;
    fn neg(self) -> Self::Output {
        match self {
            UP => DOWN,
            DOWN => UP,
            LEFT => RIGHT,
            RIGHT => LEFT
        }
    }
}