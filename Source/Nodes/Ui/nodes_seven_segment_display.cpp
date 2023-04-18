#include "Include/Nodes/Ui/nodes_seven_segment_display.h"

#include "Include/Elements/Ui/seven_segment_display.h"
#include "Include/UI/tablewidget.h"


namespace nodes::Ui {

class SevenSegmentDisplayWidget : public QGraphicsItem
{
public:
    SevenSegmentDisplayWidget()
      : QGraphicsItem{}
    {
      createSegments();
    }

    QRectF boundingRect() const override { return m_boundingRect; }

    void paint(QPainter *painter, QStyleOptionGraphicsItem const *option, QWidget *widget) override
    {
        (void)option;
        (void)widget;

        QBrush const ON{ Qt::black };
        QBrush const OFF{ QColor(90, 90, 90, 255) };

        painter->translate(10.0, 10.0);
        painter->setPen(Qt::NoPen);

        painter->setBrush(m_states[0] ? ON : OFF);
        painter->drawPolygon(m_segments[0]);

        painter->setBrush(m_states[1] ? ON : OFF);
        painter->drawPolygon(m_segments[1]);

        painter->setBrush(m_states[2] ? ON : OFF);
        painter->drawPolygon(m_segments[2]);

        painter->setBrush(m_states[3] ? ON : OFF);
        painter->drawPolygon(m_segments[3]);

        painter->setBrush(m_states[4] ? ON : OFF);
        painter->drawPolygon(m_segments[4]);

        painter->setBrush(m_states[5] ? ON : OFF);
        painter->drawPolygon(m_segments[5]);

        painter->setBrush(m_states[6] ? ON : OFF);
        painter->drawPolygon(m_segments[6]);

        painter->setBrush(m_states[7] ? ON : OFF);
        painter->drawEllipse(m_segments[7].boundingRect());
    }

    void setState(size_t const a_index, bool const a_value) { m_states[a_index] = a_value; }

private:
    void createSegments()
    {
        // Segment A
        m_segments[0] << QPointF{ 22.0, 19.0 } << QPointF{ 38.0, 4.0 } << QPointF{ 96.0, 4.0 } << QPointF{ 111.0, 19.0 }
                      << QPointF{ 96.0, 34.0 } << QPointF{ 38.0, 34.0 };

        // Segment B
        m_segments[1] << QPointF{ 115.0, 23.0 } << QPointF{ 131.0, 37.0 } << QPointF{ 131.0, 83.0 }
                      << QPointF{ 115.0, 98.0 } << QPointF{ 99.0, 83.0 } << QPointF{ 99.0, 37.0 };

        // Segment C
        m_segments[2] << QPointF{ 115.0, 105.0 } << QPointF{ 131.0, 119.0 } << QPointF{ 131.0, 165.0 }
                      << QPointF{ 115.0, 180.0 } << QPointF{ 99.0, 165.0 } << QPointF{ 99.0, 119.0 };

        // Segment D
        m_segments[3] << QPointF{ 22.0, 183.0 } << QPointF{ 38.0, 167.0 } << QPointF{ 96.0, 167.0 }
                      << QPointF{ 111.0, 183.0 } << QPointF{ 96.0, 199.0 } << QPointF{ 38.0, 199.0 };

        // Segment E
        m_segments[4] << QPointF{ 19.0, 105.0 } << QPointF{ 35.0, 119.0 } << QPointF{ 35.0, 165.0 }
                      << QPointF{ 19.0, 180.0 } << QPointF{ 4.0, 165.0 } << QPointF{ 4.0, 119.0 };

        // Segment F
        m_segments[5] << QPointF{ 19.0, 23.0 } << QPointF{ 35.0, 37.0 } << QPointF{ 35.0, 83.0 } << QPointF{ 19.0, 98.0 }
                      << QPointF{ 4.0, 83.0 } << QPointF{ 4.0, 37.0 };

        // Segment G
        m_segments[6] << QPointF{ 22.0, 101.0 } << QPointF{ 38.0, 85.0 } << QPointF{ 96.0, 85.0 } << QPointF{ 111.0, 101.0 }
                      << QPointF{ 96.0, 116.0 } << QPointF{ 38.0, 116.0 };

        // Segment DP
        m_segments[7] << QPointF{ 133.0, 162.0 } << QPointF{ 171.0, 162.0 } << QPointF{ 171.0, 199.0 }
                      << QPointF{ 133.0, 199.0 };
    }

private:
    QPolygonF m_segments[8]{};
    QRectF m_boundingRect{ 0, 0, 194, 222 };
    bool m_states[8]{};
};

SevenSegmentDisplay::SevenSegmentDisplay()
{
    auto const widget = new SevenSegmentDisplayWidget;
    setCentralWidget(widget);

    m_widget = widget;
}

void SevenSegmentDisplay::refreshCentralWidget()
{
    if (!m_element) return;

    auto const &inputs = m_element->inputs();

    bool const A{ std::get<bool>(inputs[0].value) };
    bool const B{ std::get<bool>(inputs[1].value) };
    bool const C{ std::get<bool>(inputs[2].value) };
    bool const D{ std::get<bool>(inputs[3].value) };
    bool const E{ std::get<bool>(inputs[4].value) };
    bool const F{ std::get<bool>(inputs[5].value) };
    bool const G{ std::get<bool>(inputs[6].value) };
    bool const DP{ std::get<bool>(inputs[7].value) };

    m_widget->setState(0, A);
    m_widget->setState(1, B);
    m_widget->setState(2, C);
    m_widget->setState(3, D);
    m_widget->setState(4, E);
    m_widget->setState(5, F);
    m_widget->setState(6, G);
    m_widget->setState(7, DP);

    calculateBoundingRect();
}

void SevenSegmentDisplay::showProperties()
{
    showCommonProperties();
    showIOProperties(IOSocketsType::Inputs);
    showIOProperties(IOSocketsType::Outputs);
}


} // namespace nodes::ui
