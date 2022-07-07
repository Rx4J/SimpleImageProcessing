#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

#define DIF_OPERATOR_1(A, B, C, D) \
                        abs(A-D) + abs(B-C)

#define DIF_OPERATOR_2(B, D, E, H, x) \
                        ((B+D+E+H)/4) - x

#define DIF_OPERATOR_3(A, B, C, D, E, F, G, H) \
                        abs(A + B + C - G - F - E) \
                        + abs(A + H + G - C - D - E)

namespace img {
    class Alghorithms {
    private:
        cv::Mat &pSrc;
    public:
        Alghorithms(cv::Mat &src);
        void get_image(cv::Mat &src);
        void color_inverse();
        void border_alocation(int num);
        void math_filter();
        void median_filter();
        void resize_filter();
        void increase_image(int num);
        void decrease_image(int num);
        ~Alghorithms();
    };
} 