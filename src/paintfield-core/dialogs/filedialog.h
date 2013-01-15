#ifndef PAINTFIELD_FILEDIALOG_H
#define PAINTFIELD_FILEDIALOG_H

#include <QString>
#include <QFileDialog>
#include <QHash>

namespace PaintField {

class FileDialog
{
public:
	
	enum Mode
	{
		OpenFile,
		SaveFile
	};
	
	/**
	 * Displays a dialog and gets a file path.
	 * @param parent
	 * @param title
	 * @param mode
	 * @param filterTextToSuffixes
	 * @return 
	 */
	static QString getFilePath(QWidget *parent, const QString &title, Mode mode, const QHash<QString, QStringList> &filterTextToSuffixes);
	static QString getFilePath(QWidget *parent, const QString &title, Mode mode, const QString &filterText, const QStringList &filterSuffixes);
	
	static QString getOpenFilePath(QWidget *parent, const QString &title, const QString &filterText, const QStringList &filterSuffixes);
	static QString getSaveFilePath(QWidget *parent, const QString &title, const QString &filterText, const QString &filterSuffix);
};

} // namespace PaintField

#endif // PAINTFIELD_FILEDIALOG_H