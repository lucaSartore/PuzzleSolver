//
// Created by lucas on 26-May-23.
//
#include "../solving/puzzle_preview/PieceArray.h"
#include "../solving/puzzle_preview/PieceImage.h"
#include "opencv2/opencv.hpp"
#include "../solving/logic_piece/PieceConnection.h"
#include <iostream>

using namespace std;
using namespace cv;

void calculate_corruption_rates(){

    PieceImage images[16];

    PieceConnection::set_number_of_pieces(16);
    PieceConnection connections[16];


    PieceImage::set_origin_path(R"(C:\Users\lucas\Desktop\saved_solution\divided)");

    for(int i=0; i<16; i++){
        images[i] = PieceImage(i);
        connections[i].became(R"(C:\Users\lucas\Desktop\justCon\connections)",i);
    }


    PieceArray pa = PieceArray();
    pa.load_from_file(R"(C:\Users\lucas\Desktop\saved_solution\results\solution.bin)",images);


    for (int i=0; i<41; i++){

        float v = (float) i/40.0;
        if(i==0){
            v = -1;
        }
        //cout << v << "   ";
        pa.calculate_statistics(connections,v);
        cout << endl;
    }


}