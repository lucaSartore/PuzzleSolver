use std::fmt::format;
use std::marker::PhantomData;
use std::str::CharIndices;
use piece_group::Comparable;
use std::fs;
use std::fs::File;
use std::path::Path;
use std::io::Read;
use piece_comparing::InitializationResults::FileNotFound;

/// the state of te comparator, when the comparator is uninitialized he can't
/// compare the images
/// when it is initialized, it has loaded the data from the disk and can answer questions about
/// the pieces matching
#[derive(Copy, Clone,Debug)]
pub struct Uninitialized;
#[derive(Copy, Clone,Debug)]
pub struct Initialized;

pub trait IsState{}

impl IsState for Uninitialized{}
impl IsState for Initialized{}

// note: the library ensures that the static variable will be initialized only once, and them
// used in read only.
// given that a mutex is unnecessary, and has not been used to increase access speed
/// a vector containing all the shores of the pieces
static mut SHORES_TABLE:  Vec<u8> = vec![];
/// how many pieces are stored into the array
static mut NUMBER_OF_PIECES: usize = 0;

#[derive(Copy, Clone,Debug)]
/// zero size type that allows to compare the puzzle pieces
pub struct Comparator<T: IsState>{
    state: PhantomData<T>
}
#[derive(Debug,Copy,Clone)]
pub enum InitializationResults{
    /// the initialization when well
    Ok(Comparator<Initialized>),
    /// the file was already initialized
    AlreadyInitialized,
    /// the folder was not found
    FolderNotFound,
    /// cloud not find the file with the initialization data
    FileNotFound,
    /// the file was wrong in some shape or form
    FileHasWrongFormat
}

impl<T: IsState> Comparator<T> {
    /// try to initialize the piece, and return an error if:
    ///  - the value is already initialized
    ///  - it could not find the files
    ///  - the files where in a non correct format
    fn initialize_comparator(path: &str) -> InitializationResults{

        // make sure the file has not yet been initialized
        unsafe {
            if !SHORES_TABLE.is_empty(){
                return InitializationResults::AlreadyInitialized
            }
        }

        // count the number of files in the folder
        let number_of_pieces = match count_files_in_folder(path){
            // divide by two because in the folder there is a bin and a txt file for each piece
            Option::Some(e) => e/2,
            // return error if no folder is found
            Option::None => return InitializationResults::FolderNotFound,
        };

        // if no files are found return the error
        if number_of_pieces == 0 {
            return  InitializationResults::FileNotFound;
        }

        unsafe{
            // initialize the vector with the needed capacity
            NUMBER_OF_PIECES = number_of_pieces;
            SHORES_TABLE = Vec::with_capacity(NUMBER_OF_PIECES*NUMBER_OF_PIECES*4*4);

            for first_piece in 0..NUMBER_OF_PIECES{

                // open the of the first piece
                let file_path = format!(r"{path}\{first_piece}.bin");
                let mut file = match File::open(file_path){
                    Ok(v) => v,
                    Err(_) => {
                        NUMBER_OF_PIECES = 0;
                        SHORES_TABLE = vec![];
                        return FileNotFound;
                    }
                };
                for first_piece_side in 0..4 as usize{
                    for second_piece in 0..NUMBER_OF_PIECES{
                        for second_piece_side in 0..4 as usize{

                            // read a binary buffer
                            let mut buffer = [0u8; 4];
                            match file.read_exact(&mut buffer){
                                Result::Ok(_) => (),
                                Err(_) => {
                                    NUMBER_OF_PIECES = 0;
                                    SHORES_TABLE = vec![];
                                    return InitializationResults::FileHasWrongFormat;
                                }
                            };

                            // convert to float
                            let mut shore = f32::from_le_bytes(buffer);

                            // make sure is in the right format
                            if shore < 0. || shore > 1.{
                                NUMBER_OF_PIECES = 0;
                                SHORES_TABLE = vec![];
                                return InitializationResults::FileHasWrongFormat;
                            }

                            // mapping the  shore to a u8 value
                            shore *= 255.99;

                            // push to the vector
                            SHORES_TABLE.push(shore as u8);

                        }
                    }
                }
            }

            // to be sure everything is correct
            assert_eq!(SHORES_TABLE.len(),NUMBER_OF_PIECES*NUMBER_OF_PIECES*4*4);
        }
        return InitializationResults::Ok(Comparator::<Initialized>{
            state: PhantomData::default()
        });
    }
    /// return an initialized comparator if possible (aka if someone has already initialized the comparator)
    fn get_initialized_comparator() -> Option<Comparator<Initialized>>{
        todo!()
    }
}

impl Comparator<Initialized>{
    /// if the comparator is initialized, it can be used to compare two pieces
    fn compare(&self,piece1: u64, piece2: u64, side1: u64, side2: u64) -> u8{
        unsafe {
            // calculate the address of the shore (the two expression are equivalent)
            //let address = piece1*4*NUMBER_OF_PIECES*4 + side1*NUMBER_OF_PIECES*4 + piece2*4 +side2;
            let address = ((piece1*4 + side1)*NUMBER_OF_PIECES as u64 + piece2)*4 +side2;

            return SHORES_TABLE[address as usize];
        }
    }
}

/// return the numer of files present in a folder
fn count_files_in_folder(folder: &str) -> Option<usize> {
    let path = Path::new(folder);

    if path.is_dir() {
        let count = fs::read_dir(path)
            .expect("Failed to read directory")
            .count();
        Some(count)
    } else {
        None
    }
}

#[test]
fn test_comparator(){

    let result = Comparator::<Uninitialized>::initialize_comparator(r"..\..\dataset\test_2x3\connections");


    let comparator;
    if let InitializationResults::Ok(val) = result{
        comparator = val;
    }else{
        panic!("loading failed: {:?}",result);
    }

    println!("{}",comparator.compare(0,2,0,2));

    assert_eq!(comparator.compare(0,2,0,2),(0.669904*255.99) as u8);
    assert_eq!(comparator.compare(0,1,3,2),(0.607569*255.99) as u8);
    assert_eq!(comparator.compare(1,4,1,1),(0.307137*255.99) as u8);
    assert_eq!(comparator.compare(5,4,2,1),(0.775373*255.99) as u8);

    for p1 in 0..6{
        for p2 in 0..6{
            for s1 in 0..4{
                for s2 in 0..4{
                    assert_eq!(
                        comparator.compare(p1,p2,s1,s2),
                        comparator.compare(p2,p1,s2,s1),
                    )
                }
            }
        }
    }

    println!("{:?}",result);

}