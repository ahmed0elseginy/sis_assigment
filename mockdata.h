#pragma once

#include "filestorage.h"

/**
 * Mock data generator for testing
 * Populates database with Arabic sample data
 */
class MockDataGenerator
{
public:
    static void generateMockData(FileStorage& storage);
    
private:
    static void generateAcademicLevels(FileStorage& storage);
    static void generateColleges(FileStorage& storage);
    static void generateDepartments(FileStorage& storage);
    static void generateStudents(FileStorage& storage);
    static void generateProfessors(FileStorage& storage);
    static void generateCourses(FileStorage& storage);
    static void generateLectureHalls(FileStorage& storage);
    static void generateLaboratories(FileStorage& storage);
    static void generateAssignments(FileStorage& storage);
    static void generateGrades(FileStorage& storage);
    static void generateSchedules(FileStorage& storage);
    static void generateAttendance(FileStorage& storage);
};

