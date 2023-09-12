//todo: fix test
/*
mod tests{
    use crate::piece_array::{PieceArray, PieceArrayWrapper};
    use crate::piece_comparing::{Comparator, Initialized};
    use crate::piece_group::{HasOrientation, PieceArrayFiller, PieceGroup};
    use crate::shore::Shore;
    use crate::single_piece::SingePiece;

    #[test]
    fn test_fill_piece_array_2x2(){
        todo!();
        Comparator::<Initialized>::initialize_comparator(r"..\..\dataset\test_2x3\connections");

        let mut pieces = vec![
            SingePiece::new(4,0),
            SingePiece::new(5,3),
            SingePiece::new(3,3),
            SingePiece::new(2,0),
        ];


        let already_calculated_shores = [Shore::with_shore(255),Shore::with_shore(255),Shore::with_shore(255)];

        // remember: for this test is necessary to decidable the check on the new function. otherwise the unwrap will fail
        let mut pg = PieceGroup::<SingePiece>::new(&pieces[0],&pieces[1],&pieces[3], &pieces[2],already_calculated_shores).unwrap();

        let mut pa = PieceArray::new(2,2);

        pg.fill_piece_array(&mut pa,0,0,0);

        unsafe {

            PieceArrayWrapper::load_images_to_piece_array_wrapper(r"..\..\dataset\test_2x3\divided");

            let paw = pa.get_piece_array_wrapper();

            (*paw).generate_test_image("test.png");

            (*paw).destroy_piece_array_wrapper();

        }
    }

    #[test]
    fn test_fill_piece_array_4x4() {
        // remember:
        // the combination tested here is not actualy possible, for this test to work is necessary to
        // disable the cech inside the piece group constructor

        let already_calculated_shores = [Shore::with_shore(255),Shore::with_shore(255),Shore::with_shore(255)];

        Comparator::<Initialized>::initialize_comparator(r"..\..\dataset\test_2x3\connections");

        let mut pieces = vec![
            SingePiece::new(4,0),
            SingePiece::new(5,3),
            SingePiece::new(3,3),
            SingePiece::new(2,0),
        ];
        // remember: for this test is necessary to decidable the check on the new function. otherwise the unwrap will fail
        let mut pg0 = PieceGroup::<SingePiece>::new(&pieces[0],&pieces[1],&pieces[3], &pieces[2],already_calculated_shores).unwrap();
        let mut pg1 = PieceGroup::<SingePiece>::new(&pieces[0],&pieces[1],&pieces[3], &pieces[2],already_calculated_shores).unwrap();
        let mut pg2 = PieceGroup::<SingePiece>::new(&pieces[0],&pieces[1],&pieces[3], &pieces[2],already_calculated_shores).unwrap();
        let mut pg3 = PieceGroup::<SingePiece>::new(&pieces[0],&pieces[1],&pieces[3], &pieces[2],already_calculated_shores).unwrap();

        pg1.set_orientation(3);
        pg2.set_orientation(2);
        pg3.set_orientation(1);

        let mut pg4x4 = PieceGroup::<PieceGroup::<SingePiece>>::new(&pg0,&pg1,&pg2,&pg3,already_calculated_shores).unwrap();

        //pg4x4.set_orientation(1);

        let mut pa = PieceArray::new(4,4);

        pg4x4.fill_piece_array(&mut pa,0,0,0);

        unsafe {

            PieceArrayWrapper::load_images_to_piece_array_wrapper(r"..\..\dataset\test_2x3\divided");

            let paw = pa.get_piece_array_wrapper();

            (*paw).generate_test_image("test.png");

            (*paw).destroy_piece_array_wrapper();

        }
    }
}
*/