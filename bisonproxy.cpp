#include "bisonproxy.h"
#include "imp_yacc.h"
#include "exprtree.h"

#include <QtGlobal>

ProgramStatement::ProgramStatement()
    :tree(0)
{
}

ProgramStatement::ProgramStatement(drzewo_skladn* t)
    :tree(t)
{
}

drzewo_skladn* ProgramStatement::syntacticTree()
{
    return tree;
}

ProgramStatement* ProgramStatement::scan_buffer(const QString& buf)
{
    syntaxErrorText = QString::null;
    typeCheckFailed = false;
    textlen = 0;
    identsMap.clear();
    ProgramStatement *result = 0;
    char * b = strdup(buf.toAscii().data());
    imp_scan_string(b);

    if (impparse() == 0) {
#ifdef DEBUG3
        qDebug() << "after impparse: program=" << program << " scanned length=" << textlen;
#endif
        if (program) {
            result = new ProgramStatement(program);
            program = 0;
        }
    }

    free(b);
    return result;
}

ProgramStatement* ProgramStatement::scan_buffer(const QString& buf, ProgramVariables *vars)
{
    syntaxErrorText = QString::null;
    typeCheckFailed = false;
    textlen = 0;
    identsMap.clear();
    char * b = strdup(buf.toAscii().data());
    ProgramStatement *result = 0;

#ifdef DEBUG3
    qDebug() << "buffer: " << buf.toAscii();
#endif
    imp_scan_string(b);

    if (impparse() == 0) {
#ifdef DEBUG3
        qDebug() << "after impparse: program=" << program << " scanned length=" << textlen;
#endif
        if (program) {
            result = new ProgramStatement(program);
            program = 0;

            foreach (QString k, identsMap.keys()){
                if (!vars->map()->contains(k))
                    vars->map()->insert(k, identsMap[k]);
            }
        }
    } else {
#ifdef DEBUG3
        qDebug() << "impparse failed";
#endif
    }

    free(b);
    return result;
}

VarIndex::VarIndex()
    :tc(Undefined), next(0)
{
}

VarIndex::~VarIndex()
{
    if (next)
        delete next;
}

VarIndex VarIndex::integerIndex(unsigned int i)
{
    VarIndex vi;
    vi.tc = Array;
    vi.bv.d = Long;
    vi.bv.val = i;
    return vi;
}

VarIndex VarIndex::multiIndex(QList<unsigned int> indexes, QList<DimensionDescriptor> dimensions)
{
    VarIndex vi;
    vi.tc = Array;
    vi.bv.d = Long;

    int result = 0;
    unsigned int dimensionShift = 1;
    bool error = false;

    while(!error && indexes.size() > 0){
        DimensionDescriptor dimension = dimensions.back();
        dimensions.pop_back();
        unsigned int index = indexes.back() - dimension.getFirstIndex();
        indexes.pop_back();

        if (index < dimension.getSize()){
            result += index * dimensionShift;
            dimensionShift *= dimension.getSize();
        } else {
            error = true;
            result = -1;
        }

        qDebug() << "indexes.back() =" << index << "dimensionsSizes.back() =" << dimension << "result" << result;
    }

    qDebug() << "index result:" << result;
    vi.bv.val = result;
    return vi;
}

ProgramVariables::ProgramVariables()
{
    m_map = new IdentsMap();
}

ProgramVariables::~ProgramVariables()
{
    foreach (ident_val_t *id, *m_map) {
        delete id;
    }
    m_map->clear();
    delete m_map;
}

IdentsMap *ProgramVariables::map()
{
    return m_map;
}

ident_val_t* ProgramVariables::byName(QString n)
{
    ident_val_t *found;

    if (m_map->contains(n)) {
        found = m_map->value(n);
    } else {
        qDebug() << "new variable in program variables" << n;
        found = new ident_val_t(n.toLocal8Bit().constData(), 0L);
        m_map->insert(n, found);
    }

    return found;
}

BaseValue& ProgramVariables::getVariableValue(const QString &name)
{
    ident_val_t *var = byName(name);
    return var->v.bval;
}

void ProgramVariables::setVariableValue(const QString &name, BaseValue v)
{
    ident_val_t *var = byName(name);
    if (var->t.tc == Variable) {
        var->setValue(v);
        emit valueChanged(name);
    } else {
#ifdef DEBUGX
        qDebug("%s is not a simple variable", qPrintable(name));
#endif
    }
}

BaseValue& ProgramVariables::getIndexedVariableValue(const QString& name, VarIndex i)
{
    ident_val_t *var = byName(name);
    if ((i.tc == Array) && (var->t.tc == Array)) {
        QVector<BaseValue> *vector = var->v.indval;
        long ind = i.bv.val;

        if ((ind < vector->size()) && (ind >= 0)) {
            return (*vector)[ind];
        } else {
            emit indexOutOfBounds(name);
        }
    }

    return nullBaseValue;
}

void ProgramVariables::setIndexedVariableValue(const QString& name, VarIndex i, BaseValue v)
{
    ident_val_t *var = byName(name);
    if ((i.tc == Array) && (var->t.tc == Array)) {
        QVector<BaseValue> *vector = var->v.indval;
        long ind = i.bv.val;
#ifdef DEBUGX
        qDebug("vector.size=%i index=%li", vector->size(), ind);
#endif
        if ((ind < vector->size()) && (ind >= 0)) {
            (*vector)[ind] = v;
            emit valueChanged(name);
        } else {
            emit indexOutOfBounds(name);
#ifdef DEBUGX
            qDebug("wrong index %ld for variable %s", ind, qPrintable(name));
#endif
        }
    } else {
#ifdef DEBUGX
        qDebug("%s is not an array variable", qPrintable(name));
#endif
    }
}
