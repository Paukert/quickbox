#include "readcard.h"

#include "../og/timems.h"

#include <siut/sicard.h>

#include <qf/core/log.h>

#include <QSqlRecord>
#include <QJsonDocument>

namespace quickevent {
namespace core {
namespace si {

ReadPunch::ReadPunch(const QVariantList &var_list)
{
	setCode(var_list.value(0).toInt());
	setTime(var_list.value(1).toInt());
	setMsec(var_list.value(2).toInt());
	//setDay(var_list.value(3).toInt());
	//setWeek(var_list.value(4).toInt());
}

QVariantList ReadPunch::toVariantList() const
{
	return QVariantList() << code() << time() << msec();// << dayOfWeek() << weekCnt();
}

QString ReadPunch::toJsonArrayString() const
{
	QStringList sl;
	for(auto v : toVariantList()) {
		sl << v.toString();
	}
	return '[' + sl.join(", ") + ']';
}

ReadCard::ReadCard(const QSqlRecord &rec)
{
	//setCardId(rec.value("cards.id").toInt());
	setRunId(rec.value("runId").toInt());
	setStationNumber(rec.value("stationNumber").toInt());
	setCardNumber(rec.value("siId").toInt());
	setCheckTime(rec.value("checkTime").toInt());
	setStartTime(rec.value("startTime").toInt());
	setFinishTime(rec.value("finishTime").toInt());
	setFinishTimeMs(0);
	auto jsd = QJsonDocument::fromJson(rec.value("punches").toString().toUtf8());
	QVariantList punchlst;
	QVariantList lst = jsd.toVariant().toList();
	for(auto v : lst) {
		ReadPunch punch(v.toList());
		punchlst << punch;
	}
	this->setPunches(punchlst);
}

int ReadCard::punchCount() const
{
	return punches().count();
}

ReadPunch ReadCard::punchAt(int i) const
{
	QVariantMap m = punches().value(i).toMap();
	return ReadPunch(m);
}

QList<ReadPunch> ReadCard::punchList() const
{
	QList<ReadPunch> ret;
	for (int i = 0; i < punchCount(); ++i) {
		ret << punchAt(i);
	}
	return ret;
}
/*
int ReadCard::timeMs() const
{
	int ret = 0xeeee * 1000;
	if(finishTime() != 0xeeee && startTime() != 0xeeee) {
		ret = quickevent::core::og::TimeMs::fixTimeWrapAM(0, (finishTime() - startTime()) * 1000);
		ret += finishTimeMs();
	}
	return ret;
}
*/
static QString secToStr(int sec)
{
	int hr = sec / 60 / 60;
	int min = (sec / 60) % 60;
	sec = sec % 60;
	QString ret("%1:%2:%3");
	return ret.arg(hr, 2, 10, QLatin1Char('0')).arg(min, 2, 10, QLatin1Char('0')).arg(sec, 2, 10, QLatin1Char('0'));
}

QString ReadCard::toString() const
{
	QString ret;
	QStringList punch_lst;
	for(auto v : punches()) {
		ReadPunch p(v.toMap());
		punch_lst << QString("[%1, %2]").arg(p.code()).arg(secToStr(p.time()));
	}
	ret += QString("SI: %1, run_id: %2, check: %3, start: %4, finish: %5, %6 punches: %7")
			.arg(cardNumber())
			.arg(runId())
			.arg(secToStr(checkTime()))
			.arg(secToStr(startTime()))
			.arg(secToStr(finishTime()))
			.arg(punches().count())
			.arg(punch_lst.join(", "));
	return ret;
}

}}}
