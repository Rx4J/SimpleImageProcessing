#include "alghorithms.h"
#include <iostream>
#include <vector>
#include "mpi.h"

#define IMAGE_FORMAT ".bmp"

void image_processor(cv::Mat &src, std::vector<int> &input) {
    img::Alghorithms alghoritms(src);
    for (size_t i = 0; i < input.size(); i++) {
        switch (input.data()[i]) {
        case 1:
            alghoritms.border_alocation(1);
            break;
        case 2:
            alghoritms.border_alocation(2);
            break;
        case 3:
            alghoritms.border_alocation(3);
            break;
        case 4:
            alghoritms.color_inverse();
            break;
        case 5:
            alghoritms.math_filter();
            break;
        case 6:
            alghoritms.median_filter();
            break;
        case 7:
            alghoritms.resize_filter();
            break;
        case 8:
            alghoritms.increase_image(2);
            break;
        case 9:
            alghoritms.decrease_image(2);
            break;
        default:
            break;
        }
    }
}

int main(int argc, char** argv) {
    cv::Mat image;
    std::vector<int> input;
    if(argc < 2) {
        std::cerr << "Не заданы нужные аргументы программы!\n";
        std::cerr << "Пример: ./CourseWork имя_файла" << IMAGE_FORMAT << "\n";
        return 0;
    }
    int cols, rows, type, option_size;
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    if(rank == 0) {
        while (true) {
            int tmp;
            std::string options = "Выбраны опции: ";
            if (!input.empty()) {
                for (size_t i = 0; i < input.size(); i++) {
                    options += std::to_string(input.data()[i]);
                    options += " ";
                }
                std::cout << options << "\n";
            }
            
            std::cout << "Выберете вариант задания:\n"
            << "1  | Выделение границ элементов(1)\n"
            << "2  | Выделение границ элементов(2)\n"
            << "3  | Выделение границ элементов(3)\n"
            << "4  | Инвертировать изображение\n"
            << "5  | Подавление шумов(Усред. знач.)\n"
            << "6  | Подавление шумов(Медианный фильтр)\n"
            << "7  | Улучшение изображения(уменьш. и увелич.)\n"
            << "8  | Увеличить изображение в 2 раза\n"
            << "9  | Уменьшить изображение в 2 раза\n"
            << "0  | Выход\n";
            std::cout << "> ";
            std::cin >> tmp;
            std::cout << "\n";
            if(tmp > 0) input.push_back(tmp);
            else break;
        }
        
        image = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
        if(image.empty()) {
            std::cerr << "Не удалось открыть изображение!\n";
            return -1;
        }
        
        MPI_Wtime();
        if(size < 2) {
            image_processor(image, input);
        } else {
            if(image.rows%size != 0) {
                std::cerr << "Невозможно разбить изображение на ровные части!\n";
                std::cerr << "Есть вероятность появления шумов и различных артефактов.\n";
            }
            option_size = input.size();
            cols = image.cols;
            rows = image.rows/size;
            type = image.type();
            std::cout << "Вес части изображения: " << (cols*cols)/size << " байтов\n";
        }
    }
    if(size > 1) {
        MPI_Bcast(&option_size, 1, MPI_INT, 0, MPI_COMM_WORLD);
        if(rank != 0) input = std::vector<int>(option_size);
        MPI_Bcast(input.data(), input.size(), MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&cols, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&rows, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Bcast(&type, 1, MPI_INT, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        cv::Mat tmp(rows, cols, type);
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Scatter(image.data, (cols*cols)/size, MPI_UNSIGNED_CHAR,
                    tmp.data,   (cols*cols)/size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
        MPI_Barrier(MPI_COMM_WORLD);
        image_processor(tmp, input);
        if(cols != tmp.cols) {
            cols = tmp.cols;
            if(rank == 0) image = cv::Mat(cols, cols, type);
        } 
        MPI_Barrier(MPI_COMM_WORLD);
        MPI_Gather(tmp.data,   (cols*cols)/size, MPI_UNSIGNED_CHAR,
                   image.data, (cols*cols)/size, MPI_UNSIGNED_CHAR, 0, MPI_COMM_WORLD);
    }
    if(rank == 0 && (!input.empty())) {
        std::string newimg_name = (std::string)"new_image"+(std::string)IMAGE_FORMAT;
        std::cout << "Затраченное время: " << MPI_Wtime() << "\n";
        cv::imwrite(newimg_name, image);
        while(true) {
            cv::namedWindow("image", cv::WINDOW_NORMAL);
            cv::imshow("image", image);
            if(cv::waitKey( 1 ) == 27) break;
        }
        cv::destroyAllWindows();
    }
    MPI_Finalize();
    return 0;
}