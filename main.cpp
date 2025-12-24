#include "sisapplication.h"
#include "mockdata.h"
#include <QApplication>
#include <QMessageBox>
#include <QInputDialog>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    app.setApplicationName("Student Information System");
    app.setOrganizationName("NCTU");
    
    // Initialize file storage
    FileStorage& storage = FileStorage::instance();
    if (!storage.initialize()) {
        QMessageBox::critical(nullptr, "Initialization Error", 
                             "Failed to initialize application storage!");
        return 1;
    }
    
    // Check if data already exists, if not generate mock data
    QStringList students = storage.getAllStudents();
    if (students.isEmpty()) {
        int ret = QMessageBox::question(nullptr, "Initialize Data", 
                                       "No data found. Would you like to load sample Arabic data?\n\n"
                                       "لم يتم العثور على بيانات. هل تريد تحميل بيانات عربية تجريبية؟",
                                       QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes) {
            MockDataGenerator::generateMockData(storage);
            QMessageBox::information(nullptr, "Data Loaded", 
                                    "Sample data has been loaded successfully!\n\n"
                                    "تم تحميل البيانات التجريبية بنجاح!");
        }
    }
    
    // Simple role selection dialog (can be enhanced with login)
    QStringList roles = {"Student", "Professor", "Administration"};
    bool ok;
    QString role = QInputDialog::getItem(nullptr, "Select Role", 
                                         "Please select your role:", roles, 0, false, &ok);
    
    if (!ok) {
        return 0;
    }
    
    SISApplication window;
    window.setupRoleBasedAccess(role);
    window.show();
    
    return app.exec();
}
