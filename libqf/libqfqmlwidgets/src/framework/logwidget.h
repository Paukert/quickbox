#ifndef QF_QMLWIDGETS_FRAMEWORK_LOGWIDGET_H
#define QF_QMLWIDGETS_FRAMEWORK_LOGWIDGET_H

#include "dockablewidget.h"

class QAbstractButton;
class QTableView;

namespace qf {
namespace core {
class LogEntryMap;
namespace model { class LogTableModel; }
}
}

namespace qf {
namespace qmlwidgets {
namespace framework {

class LogFilterProxyModel;

namespace Ui {
class LogWidget;
}

class QFQMLWIDGETS_DECL_EXPORT LogWidget : public qf::qmlwidgets::framework::DockableWidget
{
	Q_OBJECT
private:
	typedef qf::qmlwidgets::framework::DockableWidget Super;
public:
	explicit LogWidget(QWidget *parent = 0);
	~LogWidget();

	Q_SLOT void addLog(qf::core::Log::Level severity, const QString& category, const QString &file, int line, const QString& msg, const QDateTime& time_stamp, const QVariant &user_data = QVariant());
	Q_SLOT void addLogEntry(const qf::core::LogEntryMap &le);

	void clear();
protected:
	virtual void setLogTableModel(qf::core::model::LogTableModel *m);
	qf::core::model::LogTableModel* logTableModel();
	//bool isModelLoaded() const;
	Q_SLOT void tresholdChanged(int index);
	Q_SLOT void filterStringChanged(const QString &filter_string);
	QAbstractButton* tableMenuButton();
	QTableView* tableView() const;
private slots:
	void on_btClearLog_clicked();
	void on_btResizeColumns_clicked();
private:
	Ui::LogWidget *ui;
	qf::core::model::LogTableModel* m_logTableModel = nullptr;
	LogFilterProxyModel* m_filterModel = nullptr;
};


} // namespace framework
} // namespace qmlwiggets
} // namespace qf
#endif // QF_QMLWIDGETS_FRAMEWORK_LOGWIDGET_H