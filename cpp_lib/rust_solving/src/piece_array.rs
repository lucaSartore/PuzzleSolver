use std::fmt::{Debug, Formatter};
use crate::single_piece::SingePiece;
use crate::piece_comparing::Comparator;


#[repr(C)]
struct PieceArrayWrapper{
    _dummy: bool // useless value to disable warning (has no impact on the performance, since it uses this type only as a pointer)
}

// IMPORTANT: this part of the code cannot be compiled with the minWG toolchain,
// the msvc tool chain is commended
// you can see the tool chain you have install with the command: `rustup show`
// the tool chain `stable-x86_64-pc-windows-msvc` is the only one that has been tested, and is
// guaranteed to work

#[link(name="../cmake-build-debug/PieceArrayLink", kind = "static")]
extern "C"{
    /// create a new piece array with dimensions size_x x size_y and it will be filled up with the pieces contained in the pieces array,
    /// following "reading order" (left to right, and then top to bottom)
    fn create_piece_array_wrapper(size_x: u64, size_y: u64, pieces: *mut SingePiece) -> *mut PieceArrayWrapper;

    /// generate an image
    fn generate_test_image(piece_array_wrapper: *mut PieceArrayWrapper);

    /// deallocate the memory taken by the array
    fn destroy_piece_array_wrapper(to_destroy: *mut PieceArrayWrapper);

    /// load the images of the piece from the specified path
    fn load_images_to_piece_array_wrapper(path: *const libc::c_char);

    /// delete the images the piece had accumulated
    fn free_images_of_piece_array_wrapper();

    /// send a preview image to the c# app for the user to see
    fn generate_preview_image(piece_array_wrapper: *mut PieceArrayWrapper);

    /// return the shore (aka how well sed pieces fit together) of an image
    fn get_shore(piece_array_wrapper: *mut PieceArrayWrapper) -> f32;
}



/// an array of pieces that represent a possible solution,
/// the struct can comunicate with the C++ library, and offer the following functionalities:
/// - Can generate a preview image and send it to the c# lib to be analized
/// - Can Generate an image for shoring, and return a shore that tels how well sed pieces work together
pub struct PieceArray{
    dim_x: u64,
    dim_y: u64,
    pieces: Vec<SingePiece>,
}

impl PieceArray {
    /// get a reference to a piece at a specific coordinate
    pub fn get_piece(&self, x: u64, y: u64) -> Result<&SingePiece,()>{
        if x >= self.dim_x{
            return Result::Err(());
        }
        if y >= self.dim_y{
            return Result::Err(());
        }

        return Ok(&self.pieces[(y*self.dim_x+x) as usize]);
    }
    /// set a piece to a specific coordinates
    pub fn set_piece(&mut self, x: u64, y: u64, to_be_set: SingePiece) -> Result<(),()>{
        if x >= self.dim_x{
            return Result::Err(());
        }
        if y >= self.dim_y{
            return Result::Err(());
        }

        self.pieces[(y*self.dim_x+x) as usize] = to_be_set;

        return Ok(());
    }

    /// return a new piece array with the specified dimensions
    pub fn new(dim_x: u64, dim_y: u64) -> Self{

        let size = dim_y*dim_x;

        let mut vec = Vec::with_capacity((dim_x*dim_y) as usize);

        for _ in 0..size{
            vec.push(SingePiece::new(0,0))
        }

        Self{
            dim_x,
            dim_y,
            pieces: vec
        }
    }
}


impl Debug for PieceArray {
    fn fmt(&self, f: &mut Formatter<'_>) -> std::fmt::Result {
        let mut s = String::new();

        s += format!("Piece Array with dimensions {}x{}\n",self.dim_x,self.dim_y).as_str();


        for y in 0..self.dim_y{
            for x in 0..self.dim_x{
                println!("{x}-{y}");
                let default_cell = SingePiece::new(0,0);
                let piece = self.get_piece(x,y).unwrap_or(&default_cell);
                s += format!("  {:>4}-{}  |",piece.get_id(), piece.get_orientation()).as_str();
            }
            s += "\n";
        }

        write!(f, "{s}")
    }
}

mod testing{
    use std::ffi::CString;
    use piece_array::{create_piece_array_wrapper, destroy_piece_array_wrapper, generate_test_image, load_images_to_piece_array_wrapper, PieceArray};
    use piece_comparing::{Comparator, Initialized};
    use crate::single_piece::SingePiece;



    #[test]
    fn test_piece_array(){

        Comparator::<Initialized>::initialize_comparator(r"..\..\dataset\test_2x3\connections");

        let mut piece_array = PieceArray::new(3,5);
        for x in 0..3{
            for y in 0..5{
                piece_array.set_piece(x,y,SingePiece::new(y,x)).unwrap();
            }
        }
        println!("{:?}",piece_array);

        for x in 0..3{
            for y in 0..5{
                let piece = piece_array.get_piece(x,y).unwrap();
                assert_eq!(x,piece.get_orientation());
                assert_eq!(y,piece.get_id());
            }
        }
    }

    #[test]
    fn test_link(){

        Comparator::<Initialized>::initialize_comparator(r"..\..\dataset\test_2x3\connections");

        unsafe {
            // the path where the program can find the images
            let path = CString::new(r"..\..\dataset\test_2x3\divided").expect("CString::new failed");
            let path_ptr = path.as_ptr();

            // load the images
            load_images_to_piece_array_wrapper(path_ptr);


            let mut pieces = vec![
                SingePiece::new(4,0),
                SingePiece::new(5,3),
                SingePiece::new(3,3),
                SingePiece::new(2,0),
            ];

            // create a piece array
            let pa = create_piece_array_wrapper(2,2,pieces.as_mut_ptr());

            // generate an image
            generate_test_image(pa);

            // free the image in memory
            destroy_piece_array_wrapper(pa)
        }
    }

}
