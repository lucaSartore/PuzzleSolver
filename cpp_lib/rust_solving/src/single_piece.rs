/// one single piece, keep the ID and his rotation
#[derive(Debug)]
#[repr(C)]
pub struct SingePiece{
    id: u64,
    orientation: u64
}

impl SingePiece {
    pub fn get_id(&self) -> u64{
        self.id
    }
    pub fn get_orientation(&self) -> u64{
        self.orientation
    }
    pub fn new(id: u64, orientation: u64) -> Self{
        assert!(orientation < 4);
        Self{
            id,
            orientation
        }
    }
}