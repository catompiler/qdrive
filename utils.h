#ifndef UTILS_H
#define UTILS_H


class QPainter;
class QString;

namespace utils{

namespace paint{

enum TextPosition {
    TEXT_POS_TOP_LEFT = 0,
    TEXT_POS_TOP_RIGHT,
    TEXT_POS_BOTTOM_LEFT,
    TEXT_POS_BOTTOM_RIGHT,
    TEXT_POS_TOP_CENTER,
    TEXT_POS_BOTTOM_CENTER,
    TEXT_POS_LEFT_CENTER,
    TEXT_POS_RIGHT_CENTER
};

void drawTextAt(QPainter* painter, const QString& text, int x, int y, TextPosition pos);

}

}


#endif // UTILS_H
