#ifndef PLOT_TEST_STEPANPLOT_DATA_FRAME_H
#define PLOT_TEST_STEPANPLOT_DATA_FRAME_H

namespace stepan_plot {

    namespace data_frame {
//------------------------------------------------------------------------------------------------------------
        /**\brief - класс рамка
         * */
        class Frame {
        public:
            unsigned int h = 512;   /* Высота рамки */
            unsigned int w = 512;   /* Ширина рамки */

            /** \brief Инициализация параметров рамки по умолчанию
             * */
            Frame() {}

            /** \brief Инициализация параметров рамки
             * \param height высота
             * \param width широта
             */
            Frame(unsigned int height, unsigned int width) {
                h = height;
                w = width;
            }
        };
//------------------------------------------------------------------------------------------------------------
        /**\brief - класс позиции нового окна
         * */
        class Position {
        private:
            /* Размеры дисплея, по умолчанию */
            unsigned int display_height = 1080;
            unsigned int display_width = 1920;

        public:
            unsigned int X = 0;  /* Положение по Х */
            unsigned int Y = 0;  /* Положение по У */

            /** \brief Инициализация параметров рамки по умолчанию
             * */
            Position() {}

            /** \brief Инициализация параметров рамки
             * \param x смещение по х
             * \param y смещение по y
             */
            Position(unsigned int x, unsigned int y) {
                X = x;
                Y = y;
            }

            /** \brief Переход к следующему положению
             * \param fr рамка
             */
            void next(Frame fr) {
                if ((X + fr.w) <= display_width) {
                    X += fr.w;
                    return;
                }

                if ((Y + fr.h) <= display_height) {
                    Y += fr.h;
                    X = 0;
                    return;
                }

                X = 0;
                Y = 0;
            }
        };
//------------------------------------------------------------------------------------------------------------
        /** \brief Характеристики окна в котором строим график
         * */
        class Ortho {
        public:
            double left;
            double right;
            double bottom;
            double top;
            double near = 0.0;
            double far = 100.0;
        };
    }

};

#endif // PLOT_TEST_STEPANPLOT_DATA_FRAME_H
