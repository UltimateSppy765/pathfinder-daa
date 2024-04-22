#include "pathfinder.h"
#include <QMessageBox>
#include <QtConcurrent/QtConcurrentRun>

using namespace utils;

Matrix<point_state> PathFinder::generateMatrix() {
    Matrix<point_state> matrix(Pair(rowSlider->value(), columnSlider->value()));
    for (int i(0); i < rowSlider->value(); ++i) {
        for (int j(0); j < columnSlider->value(); ++j) {
            if (mazeTable->selectionModel()->isSelected(mazeTable->model()->index(i, j)))
                matrix[Pair(i, j)] = CAN_VISIT;
            else
                matrix[Pair(i, j)] = CANNOT_VISIT;
        }
    }
    return matrix;
}

void PathFinder::backtrack() {
    if (found)
        results.clear(); // Start with a fresh vector in case user re-runs the algo
    auto matrix{ generateMatrix() };
    QElapsedTimer timer;
    timer.start();
    found = track(matrix, Pair(source->row(), source->column()), Pair(destination->row(), destination->column()), results);
    nanosElapsed = timer.nsecsElapsed();
}

void PathFinder::branchandbound() {
    if (found)
        results.clear(); // Start with a fresh vector in case user re-runs the algo
    auto matrix{ generateMatrix() };
    vector<Pair> current{};
    QElapsedTimer timer;
    timer.start();
    bound(matrix, Pair(source->row(), source->column()), Pair(destination->row(), destination->column()), results, current);
    nanosElapsed = timer.nsecsElapsed();
    found = !results.empty();
    if (found) results.pop_back();
}

void PathFinder::postPathFind() {
    if (found) {
        auto model{ mazeTable->model() };
        int pos{ 1 };
        model->setData(*source, QString::number(pos++));
        for (auto it{ results.rbegin() }; it != results.rend(); ++it) {
            model->setData(model->index(it->first, it->second), QString::number(pos++));
        }
    }
    resultsLabel->setText(resultString.arg(found ? "" : " not", QString::number(nanosElapsed, 'g', 6)));
    resultsLabel->show();
    optionsPane->setEnabled(true);
    delete workerWatcher;
}

PathFinder::PathFinder(QWidget *parent)
    : QMainWindow(parent)
{
    setupUi(this);

    // Makes text in all cells centered
    auto itemModel{ new QTableWidgetItem() };
    itemModel->setTextAlignment(Qt::AlignCenter);
    mazeTable->setItemPrototype(itemModel);

    // Hide results label, we don't have a result yet
    resultsLabel->hide();
}

PathFinder::~PathFinder()
{}

void PathFinder::on_rowSlider_valueChanged(int value) {
    if (found) on_resetMazeButton_clicked(); // Reset on changing rows
    rowCountLabel->setText(QString("Rows: %1").arg(value));
    mazeTable->setRowCount(value);
    if (source && !mazeTable->model()->checkIndex(*source)) {
        delete source;
        source = nullptr;
    }
    if (destination && !mazeTable->model()->checkIndex(*destination)) {
        delete destination;
        destination = nullptr;
    }
}

void PathFinder::on_columnSlider_valueChanged(int value) {
    if (found) on_resetMazeButton_clicked(); // Reset on changing columns
    columnCountLabel->setText(QString("Columns: %1").arg(value));
    mazeTable->setColumnCount(value);
    if (source && !mazeTable->model()->checkIndex(*source)) {
        delete source;
        source = nullptr;
    }
    if (destination && !mazeTable->model()->checkIndex(*destination)) {
        delete destination;
        destination = nullptr;
    }
}

void PathFinder::on_findPathButton_clicked() {
    if (!source || !destination) {
        QMessageBox dialog{ this };
        dialog.setWindowTitle("Error");
        dialog.setText("Source or destination point is not set.");
        dialog.exec();
        return;
    }
    if (found) on_resetMazeButton_clicked();
    optionsPane->setEnabled(false);
    mazeTable->setEnabled(false);
    auto selectedAlgo{ algorithmComboBox->currentText() };
    if (selectedAlgo == "Backtracking") {
        workerWatcher = new QFutureWatcher<void>;
        connect(workerWatcher, &QFutureWatcher<void>::finished, this, &PathFinder::postPathFind);
        worker = QtConcurrent::run(&PathFinder::backtrack, this);
        workerWatcher->setFuture(worker);
    }
    else if (selectedAlgo == "Branch & Bound") {
        workerWatcher = new QFutureWatcher<void>;
        connect(workerWatcher, &QFutureWatcher<void>::finished, this, &PathFinder::postPathFind);
        worker = QtConcurrent::run(&PathFinder::branchandbound, this);
        workerWatcher->setFuture(worker);
    }
    else {
        QMessageBox dialog2{ this };
        dialog2.setWindowTitle("Error");
        dialog2.setText("Algorithm not implemented.");
        dialog2.exec();
        optionsPane->setEnabled(true);
        mazeTable->setEnabled(true);
    }
}

void PathFinder::on_resetMazeButton_clicked() {
    if (found) {
        auto model{ mazeTable->model() };
        for (auto const& i : results) {
            model->setData(model->index(i.first, i.second), "");
        }
        if (*source == *destination)
            model->setData(*source, "SD");
        else {
            model->setData(*source, "S");
            model->setData(*destination, "D");
        }
        results.clear();
        found = false;
    }
    mazeTable->setEnabled(true);
    resultsLabel->hide();
}

void PathFinder::on_markSourceButton_clicked() {
    if (found) return;
    auto idx{ mazeTable->currentIndex() };
    mazeTable->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Select);
    auto model{ mazeTable->model() };
    if (source) {
        if (destination && *source == *destination)
            model->setData(*destination, "D");
        else
            model->setData(*source, "");
        delete source;
    }
    source = new QModelIndex(idx);
    if (destination && *source == *destination)
        model->setData(idx, "SD");
    else
        model->setData(idx, "S");
}

void PathFinder::on_markDestinationButton_clicked() {
    if (found) return;
    auto idx{ mazeTable->currentIndex() };
    mazeTable->selectionModel()->setCurrentIndex(idx, QItemSelectionModel::Select);
    auto model{ mazeTable->model() };
    if (destination) {
        if (source && *source == *destination)
            model->setData(*destination, "S");
        else
            model->setData(*destination, "");
        delete destination;
    }
    destination = new QModelIndex(idx);
    if (source && *source == *destination)
        model->setData(idx, "SD");
    else
        model->setData(idx, "D");
}

void PathFinder::on_mazeTable_itemSelectionChanged() {
    auto model{ mazeTable->model() };
    if (source && !mazeTable->selectionModel()->isSelected(*source)) {
        model->setData(*source, "");
        delete source;
        source = nullptr;
    }
    if (destination && !mazeTable->selectionModel()->isSelected(*destination)) {
        model->setData(*destination, "");
        delete destination;
        destination = nullptr;
    }
}