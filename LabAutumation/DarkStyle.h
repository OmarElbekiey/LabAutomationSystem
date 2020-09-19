#pragma once
#pragma warning(push, 0)
#include <QApplication>
#include <QFile>
#include <QFont>
#include <QProxyStyle>
#include <QStyleFactory>
#pragma warning(pop)


class DarkStyle : public QProxyStyle
{
	Q_OBJECT

public:
	DarkStyle();
	explicit DarkStyle(QStyle* style);
	void polish(QApplication* app) override;
	QPalette standardPalette() const override;

private:
	QStyle* styleBase(QStyle* style = Q_NULLPTR) const;
};