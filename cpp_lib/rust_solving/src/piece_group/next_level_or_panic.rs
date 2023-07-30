use crate::piece_group::{AddShoreOfSubComponents, CanCreateSet, Comparable, HasKnownLevel, HasOrientation, IsSubComponent, PieceArrayFiller, PieceGroup};
use crate::piece_group_holder::PieceGroupHolder;
use crate::single_piece::SingePiece;
use super::super::solver::solve;


/// the `solve` function need to be a generic recursive function...
/// such a function would generate infinite versions of itself making it impossible to compile
/// this trait will decide on a type by type whether or not to call the next function... preventing this issue
/// this trait also group all the trait necessary fro the function to work
pub trait NextLevelOrPanic: Clone + HasOrientation + Send + Comparable + IsSubComponent + HasKnownLevel + PieceArrayFiller + Sync + CanCreateSet<Self> + AddShoreOfSubComponents{
    fn next_or_panic(pgh: &PieceGroupHolder<PieceGroup<Self>>);
}

/// this macro implement the next or panic trait for a type, and specific it implements the next version
macro_rules! implement_next {
    ($i: ty) => {
        impl NextLevelOrPanic for $i{
            fn next_or_panic(pgh: &PieceGroupHolder<PieceGroup<Self>>) {
                solve::<PieceGroup<Self>>(pgh);
            }
        }
    }
}

/// this macro implement the next or panic trait for a type, and specific it implements the panic version
macro_rules! implement_panic {
    ($i: ty) => {
        impl NextLevelOrPanic for $i{
            fn next_or_panic(pgh: &PieceGroupHolder<PieceGroup<Self>>) {
                panic!("the compiled limit of the recursive generic function has been exceeded!")
            }
        }
    }
}

// implement the next function for the first 10 items and then implement panic
implement_next!(SingePiece);
implement_next!(PieceGroup<'_,SingePiece>);
implement_next!(PieceGroup<'_,PieceGroup<'_,SingePiece>>);
implement_next!(PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,SingePiece>>>);
implement_next!(PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,SingePiece>>>>);
implement_next!(PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,SingePiece>>>>>);
implement_next!(PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,SingePiece>>>>>>);
implement_next!(PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,SingePiece>>>>>>>);
implement_next!(PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,SingePiece>>>>>>>>);
implement_next!(PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,SingePiece>>>>>>>>>);

implement_panic!(PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,PieceGroup<'_,SingePiece>>>>>>>>>>);