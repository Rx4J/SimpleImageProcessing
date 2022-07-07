#include "alghorithms.h"
using namespace img;

Alghorithms::Alghorithms(cv::Mat &src): pSrc(src) {}

void Alghorithms::get_image(cv::Mat &src) {
    this->pSrc = src;
}

void Alghorithms::color_inverse() {
    for (size_t i = 0; i < pSrc.total(); i++) {
        pSrc.data[i] = ~pSrc.data[i];
    }
}

void Alghorithms::border_alocation(int num) {
	if(num == 1) {
		for (size_t y = 0; y < pSrc.rows-1; y++) {
			for(size_t x = 0; x < pSrc.cols-1; x++) {
        	    uchar A = pSrc.data[(y+0)*pSrc.cols+(x+0)];
			    uchar B = pSrc.data[(y+0)*pSrc.cols+(x+1)];
			    uchar C = pSrc.data[(y+1)*pSrc.cols+(x+0)];
			    uchar D = pSrc.data[(y+1)*pSrc.cols+(x+1)];
        	    uchar val = DIF_OPERATOR_1(A, B, C, D);
        	    pSrc.data[(y)*pSrc.cols+(x)] = val;
			}
		}
	} else {
		for (size_t y = 1; y < pSrc.rows-1; y++) {
			for(size_t x = 1; x < pSrc.cols-1; x++) {
        	    uchar A = pSrc.data[(y-1)*pSrc.cols+(x-1)];
			    uchar B = pSrc.data[(y+0)*pSrc.cols+(x-1)];
			    uchar C = pSrc.data[(y+1)*pSrc.cols+(x-1)];
			    uchar D = pSrc.data[(y+1)*pSrc.cols+(x+0)];
			    uchar E = pSrc.data[(y+1)*pSrc.cols+(x+1)];
			    uchar F = pSrc.data[(y+0)*pSrc.cols+(x+1)];
			    uchar G = pSrc.data[(y-1)*pSrc.cols+(x+1)];
			    uchar H = pSrc.data[(y-1)*pSrc.cols+(x+0)];
				uchar O = pSrc.data[(y+0)*pSrc.cols+(x+0)];
        	    uchar val = num == 2 ? DIF_OPERATOR_2(B, D, E, H, O) :
									DIF_OPERATOR_3(A, B, C, D, E, F, G, H);
        	    pSrc.data[(y-1)*pSrc.cols+(x-1)] = val;
			}
		}
	}   
}

void Alghorithms::math_filter() {
    for (size_t y = 1; y < pSrc.rows - 1; y++) {
	    for (size_t x = 1; x < pSrc.cols - 1; x++) {
	    	int sum = 0;
	    	for (int p = -1; p <= 1; p++) {
	    		for (int q = -1; q <= 1; q++) {
	    			int pixel = pSrc.data[(y+p)*pSrc.cols+(x+q)];
	    			sum += pixel;
	    		}
	    	}
	    	pSrc.data[(y)*pSrc.cols+(x)] = (unsigned char)(sum / 9);
	    }
    }
}

void Alghorithms::median_filter() {
    std::vector<uchar> pixel(9);
    for (size_t y = 1; y < pSrc.rows-1; y++) {
		for(size_t x = 1; x < pSrc.cols-1; x++) {
            pixel[0] = pSrc.data[(y-1)*pSrc.cols+(x-1)];
		    pixel[1] = pSrc.data[(y-1)*pSrc.cols+(x+0)];
		    pixel[2] = pSrc.data[(y-1)*pSrc.cols+(x+1)];
		    pixel[3] = pSrc.data[(y+0)*pSrc.cols+(x-1)];
		    pixel[4] = pSrc.data[(y+0)*pSrc.cols+(x+0)];
		    pixel[5] = pSrc.data[(y+0)*pSrc.cols+(x+1)];
		    pixel[6] = pSrc.data[(y+1)*pSrc.cols+(x-1)];
		    pixel[7] = pSrc.data[(y+1)*pSrc.cols+(x+0)];
            pixel[8] = pSrc.data[(y+1)*pSrc.cols+(x+1)];
            std::sort(pixel.begin(), pixel.end());
            pSrc.data[(y)*pSrc.cols+(x)] = pixel[9 / 2];
		}
	}
}

void Alghorithms::increase_image(int num) {
	cv::Mat dst((pSrc.rows)*num, (pSrc.cols)*num, pSrc.type());
	for (size_t y_src = 0, y_dst = 0; y_src < pSrc.rows; y_src++, y_dst += num) {
	    for(size_t x_src = 0, x_dst = 0; x_src < pSrc.cols; x_src++, x_dst += num) {
			for (size_t pow_y = 0; pow_y < num; pow_y++) {
				for (size_t pow_x = 0; pow_x < num; pow_x++) {
					dst.data[(y_dst+pow_y)*dst.cols+(x_dst+pow_x)] = 
										pSrc.data[(y_src)*pSrc.cols+(x_src)];
				}
			}
	    }
	}
	pSrc = dst;
}

void Alghorithms::decrease_image(int num) {
    cv::Mat dst((pSrc.rows)/num, (pSrc.cols)/num, pSrc.type());
	for (size_t y_src = 0, y_dst = 0; y_src < pSrc.rows; y_src += num, y_dst++) {
	    for(size_t x_src = 0, x_dst = 0; x_src < pSrc.cols; x_src += num, x_dst++) {
				dst.data[(y_dst)*dst.cols+(x_dst)] = 
								pSrc.data[(y_src)*pSrc.cols+(x_src)];
	    }
	}
	pSrc = dst;
}

void Alghorithms::resize_filter() {
	decrease_image(2);
	increase_image(2);
}

Alghorithms::~Alghorithms() {}
