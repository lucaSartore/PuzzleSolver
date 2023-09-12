use std::iter::zip;
use std::marker::PhantomData;

static mut NUMBER_OF_PIECES: usize = 0;
static mut ARRAY_LEN: usize = 0;
/// this struct is used to keep track of the level 1 components (aka single pieces) a piece group is made of...
/// when a piece is constructed his sub components will be the union of all the level 1 components of his 4 sub components.
/// when 4 components are merge an error could be thrown if they have at least one level 1 component in common
#[derive(Clone)]
#[derive(Debug)]
pub struct PieceBasicComponents{
    codes: Vec<u64>,
}

impl PieceBasicComponents {

    /// turn the but at the specified index to 1 using a bitwise mask
    fn set_bit(&mut self, index: usize){
        self.codes[index/64] |= 1 << (index%64);
    }

    /// merge two components together if they have no sub component that match
    /// otherwise return an error
    pub fn merge(first: &Self, second: &Self) -> Result<Self,()>{
        let mut result_vec = Vec::with_capacity(first.codes.len());

        for (f,s) in zip(first.codes.as_slice(), second.codes.as_slice()){
            // if at least one of the bits is in common it means that they are not compatible
            if f & s != 0{
                return Result::Err(())
            }
            result_vec.push(f | s);
        }

        return Result::Ok(Self{
            codes: result_vec
        });
    }

    /// create a new item... can panic if the struct is uninitialized
    pub fn new(basic_piece_id: usize) -> Self{
        let mut v;

        unsafe {
            // you must initialize struct first
            assert_ne!(NUMBER_OF_PIECES, 0);

            // buffer overflow protection
            assert!(basic_piece_id < NUMBER_OF_PIECES);

            v = Vec::with_capacity(ARRAY_LEN);
            for i in 0..ARRAY_LEN{
                v.push(0);
            }
        }

        let mut pbc = Self{
            codes: v
        };

        pbc.set_bit(basic_piece_id);

        return pbc;
    }

    /// allow to initialize the the piece by specifying how many pieces there are
    pub fn initialize(number_of_pieces: usize){
        unsafe {
            NUMBER_OF_PIECES = number_of_pieces;
            ARRAY_LEN = number_of_pieces / 64;
            if number_of_pieces % 64 != 0{
                ARRAY_LEN += 1;
            }
        }
    }
}