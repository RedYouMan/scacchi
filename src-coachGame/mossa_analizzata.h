
#ifndef MOSSA_ANALIZZATA
#define MOSSA_ANALIZZATA
#include <iostream>
#include <string>
// mossa_analizzata.h
class MossaAnalizzata
{
private:
    int n_mossa;

    // parte bianco
    std::string alu_bianco;
    std::string stock_bianco;
    float eval_prima_b;
    float eval_dopo_b;
    // parte nero
    std::string alu_nero;
    std::string stock_nero;
    float eval_prima_n;
    float eval_dopo_n;
    // commento sulla best move
    std::string commento;
    std::string best_move;

public:
    MossaAnalizzata()
        : n_mossa(0),
          eval_prima_b(0.0f),
          eval_dopo_b(0.0f),
          eval_prima_n(0.0f),
          eval_dopo_n(0.0f) {}

    ~MossaAnalizzata() = default;

    void set_n_mossa(int n) { n_mossa = n; }
    int get_n_mossa() const { return n_mossa; }

    void set_alu_bianco(const std::string &alu) { alu_bianco = alu; }
    const std::string &get_alu_bianco() const { return alu_bianco; }

    void set_stock_bianco(const std::string &stock) { stock_bianco = stock; }
    const std::string &get_stock_bianco() const { return stock_bianco; }

    void set_eval_prima_b(float eval) { eval_prima_b = eval; }
    float get_eval_prima_b() const { return eval_prima_b; }

    void set_eval_dopo_b(float eval) { eval_dopo_b = eval; }
    float get_eval_dopo_b() const { return eval_dopo_b; }

    void set_alu_nero(const std::string &alu) { alu_nero = alu; }
    const std::string &get_alu_nero() const { return alu_nero; }

    void set_stock_nero(const std::string &stock) { stock_nero = stock; }
    const std::string &get_stock_nero() const { return stock_nero; }

    void set_eval_prima_n(float eval) { eval_prima_n = eval; }
    float get_eval_prima_n() const { return eval_prima_n; }

    void set_eval_dopo_n(float eval) { eval_dopo_n = eval; }
    float get_eval_dopo_n() const { return eval_dopo_n; }

    void set_commento(const std::string &comment, char color);
    const std::string &get_commento() const { return commento; }

    void set_best_move(const std::string &move) { best_move = move; }
    const std::string &get_best_move() const { return best_move; }
};

#endif