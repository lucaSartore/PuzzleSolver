use std::fmt;

/// This struct keeps track of the shore of each possible combination of pieces,
/// need to keep track of the average shore, and the number of pieces the shore keeps track of.
/// `shore` is used to make a weighted average when combining multiple Shores.
#[derive(Clone, Copy)]
pub struct Shore {
    /// The average shore of the n represented connections.
    shore: u8,
    /// The number of connections this shore is representing.
    number: u32,
}

impl Shore {
    /// Default constructor, gives a shore with `shore` = 1 and `number` = 0.
    pub fn new() -> Self {
        Shore { shore: 1, number: 0 }
    }

    /// Create a new shore, `number` will be 1.
    pub fn with_shore(shore: u8) -> Self {
        Shore { shore, number: 1 }
    }

    /// Get the shore of the piece.
    pub fn get_shore(&self) -> u8 {
        self.shore
    }
}

use std::ops;
use libc::c_float;

impl ops::Add for Shore {
    type Output = Shore;
    /// Sum operator, this makes a weighted sum of `shore`, using `number` as weight.
    fn add(self, other: Shore) -> Shore {
        let mut result = self;
        result+=other;
        result
    }
}

impl ops::AddAssign for Shore{

    /// Sum operator, this makes a weighted sum of `shore`, using `number` as weight.
    fn add_assign(&mut self, other: Shore) {
        let other_number = other.number;
        let other_shore = other.shore;

        // If both shores are zero, return immediately to avoid division by zero.
        if self.number == 0 && other_number == 0 {
            return;
        }

        // A connection that is impossible (aka, shore is 0) has to remain impossible even if other
        // shores are greater.
        if self.shore == 0 || other_shore == 0 {
            self.shore = 0;
            self.number = 1;
            return;
        }

        // Calculate new shore with weighted average.
        let total_number = self.number + other_number;
        self.shore = ((self.shore as u32 * self.number as u32 + other_shore as u32 * other_number as u32) as f32
            / total_number as f32 + 0.5 ) as u8;
        // Sum numbers.
        self.number = total_number;
    }
}

impl fmt::Display for Shore {
    fn fmt(&self, f: &mut fmt::Formatter<'_>) -> fmt::Result {
        write!(f, "shore: {} num: {}", self.shore, self.number)
    }
}