#include <QtGui>
#include "Info.h"
#include <map>
#include <string>
class YourWidget : public QWidget
{
public:
    YourWidget(QWidget* parent=0)
        : QWidget(parent)
    {
    	Info f;
    	f.FillInfoMap();
	std::map<int,double> mymap = f.GetMap();
	
        //QHBoxLayout* buttonLay = new QHBoxLayout;
        QVBoxLayout* buttonLay = new QVBoxLayout;
	        QVBoxLayout* voltageLay = new QVBoxLayout;

        std::map<int,double>::iterator it;
	int i=0;
        for (it=mymap.begin(); it!=mymap.end(); ++it)
        {
	i++;
        //for (int i=0; i<20; ++i) {
            QPushButton* button = new QPushButton(QString::number(it->first)); //(tr("Button%1").arg(i));
            buttonLay->addWidget(button);
	     QLabel* label = new QLabel(QString::number(it->second));  //(tr("Label%1").arg(i));
            voltageLay->addWidget(label);
        }
/*
	QVBoxLayout* voltageLay = new QVBoxLayout;
        for (int i=0; i<20; ++i) {
            QLabel* label = new QLabel(tr("Label%1").arg(i));
            voltageLay->addWidget(label);
        }
*/
        QHBoxLayout* mainLay = new QHBoxLayout(this);
        mainLay->addLayout(buttonLay);
        mainLay->addLayout(voltageLay);
    }
};

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

QWidget* topLevelWidget = 0;
    YourWidget* editor = new YourWidget;
#ifdef USE_NO_SCROLL_AREA
    topLevelWidget = editor;
#else
    QScrollArea* scroller = new QScrollArea;
    scroller->setWidget(editor);
    topLevelWidget = scroller;
#endif
    topLevelWidget->show();
    topLevelWidget->raise();
    return app.exec();
}
