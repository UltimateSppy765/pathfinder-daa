/*
* Dummy header file for the sake of MOC
* to generate the corresponding MOC file
*/

#pragma once

#include <QFutureWatcher>
#include "ui_pathfinder.h"

import utilities;

using namespace std;
using namespace utils;

class PathFinder : public QMainWindow, public Ui::PathFinderClass
{
    Q_OBJECT

    QFuture<void> worker;
    QFutureWatcher<void>* workerWatcher{ nullptr };
    QModelIndex *source{ nullptr }, *destination{ nullptr };
    qint64 nanosElapsed{ 0 };
    const QString resultString{ "Path%1 found. Time Taken: %2 nanoseconds" };
    // Variables to be used in pathfinding functions
    vector<Pair> results{};
    bool found{ false };

    Matrix<point_state> generateMatrix();
    void backtrack();
    void branchandbound();
    void postPathFind();

public:
    PathFinder(QWidget *parent = nullptr);
    ~PathFinder();

private slots:
    void on_rowSlider_valueChanged(int);
    void on_columnSlider_valueChanged(int);
    void on_findPathButton_clicked();
    void on_resetMazeButton_clicked();
    void on_markSourceButton_clicked();
    void on_markDestinationButton_clicked();
    void on_mazeTable_itemSelectionChanged();
};
