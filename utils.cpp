#include "utils.h"
#include <QPainter>
#include <QString>
#include <QFontMetrics>
#include <QSize>


namespace utils{

namespace paint{

void drawTextAt(QPainter* painter, const QString &text, int x, int y, TextPosition pos)
{
    QFontMetrics fm(painter->font());
    QSize bound = fm.size(0, text);

    //qDebug() << fm.leading() << fm.ascent() << fm.descent() << fm.lineSpacing() << fm.lineWidth() << fm.strikeOutPos() << fm.overlinePos() << fm.underlinePos();

    int dx = fm.leading() - fm.minLeftBearing();
    int dy = fm.lineWidth() + fm.descent();

    switch(pos){
    case TEXT_POS_TOP_LEFT:
        painter->drawText(x - bound.width() - dx, y - dy, text);
        break;
    case TEXT_POS_TOP_RIGHT:
        painter->drawText(x + dx, y - dy, text);
        break;
    case TEXT_POS_BOTTOM_LEFT:
        painter->drawText(x - bound.width() - dx, y + bound.height() - dy, text);
        break;
    case TEXT_POS_BOTTOM_RIGHT:
        painter->drawText(x + dx, y + bound.height() - dy, text);
        break;
    case TEXT_POS_TOP_CENTER:
        painter->drawText(x + (dx - bound.width()) / 2, y - dy, text);
        break;
    case TEXT_POS_BOTTOM_CENTER:
        painter->drawText(x + (dx - bound.width()) / 2, y + bound.height() - dy, text);
        break;
    case TEXT_POS_LEFT_CENTER:
        painter->drawText(x - bound.width() - dx, y + (bound.height() - dy) / 2, text);
        break;
    case TEXT_POS_RIGHT_CENTER:
        painter->drawText(x + dx, y + (bound.height() - dy) / 2, text);
        break;
    }
}

}

}
