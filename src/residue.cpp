/**********************************************************************
residue.cpp - Handle macromolecule residues.
 
Copyright (C) 2001, 2002  OpenEye Scientific Software, Inc.
Some portions Copyright (C) 2001-2006 by Geoffrey R. Hutchison
 
This file is part of the Open Babel project.
For more information, see <http://openbabel.sourceforge.net/>
 
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation version 2 of the License.
 
This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
***********************************************************************/

/**********************************************************************
Global arrays Residue, ElemDesc and function GetResidueNumber were
obtained in part or whole from RasMol2 by Roger Sayle.
***********************************************************************/

///////////////////////////////////////////////////////////////////////////////
// File Includes
///////////////////////////////////////////////////////////////////////////////

#include "babelconfig.h"

#include "residue.h"

using namespace std;

namespace OpenBabel
{
  /** \class OBResidue
      \brief Residue information
 
      The residue information is drawn from PDB or MOL2 files (or similar), which
      track biomolecule information,
      and are stored in the OBResidue class. OBResidues are stored inside the 
      OBAtom class and OBMol classes. 
      The residue information for an atom can be requested in 
      the following way:
      \code
      OBAtom *atom;
      OBResidue *r;
      atom = mol.GetAtom(1);
      r = atom->GetResidue();
      \endcode
      The residue information for a molecule can be manipulated too:
      \code
      cout << "This molecule has " << mol.NumResidues() << " residues." << endl;
      OBResidue *r;
      r = mol.GetResidue(1);
      \endcode
  */

  ///////////////////////////////////////////////////////////////////////////////
  // Residue Functions
  ///////////////////////////////////////////////////////////////////////////////

  static unsigned int GetAtomIDNumber(const char *atomid)
  {
    if (atomid != NULL)
      {
        int ch1 = toupper(atomid[0]);
        int ch2 = toupper(atomid[1]);
        int ch3 = toupper(atomid[2]);
        int ch4 = toupper(atomid[3]);

        if (ch1 == ' ')
          {
            switch(ch2)
              {
              case 'C':

                switch(ch3)
                  {
                  case 'A':

                    if (ch4 == ' ')
                      return 1;
                    else if (ch4 == '2')
                      return 19;

                    break;

                  case ' ':
                    if (ch4 == ' ')
                      return 2;
                    break;

                  case '\'':
                    if (ch4 == ' ')
                      return 4;
                    break;

                  case '1':
                    if (ch4 == '*')
                      return 18;
                    break;

                  case '2':
                    if (ch4 == '*')
                      return 16;
                    break;

                  case '3':
                    if (ch4 == '*')
                      return 14;
                    break;

                  case '4':
                    if (ch4 == '*')
                      return 12;
                    break;

                  case '5':
                    if (ch4 == '*')
                      return 11;
                    break;

                  }

                break;

              case 'N':

                if (ch4 == ' ')
                  {
                    switch (ch3)
                      {
                      case ' ':
                        return 0;
                      case '1':
                        return 21;
                      case '2':
                        return 22;
                      case '3':
                        return 23;
                      case '4':
                        return 24;
                      case '6':
                        return 25;
                      }
                  }

                break;

              case 'O':

                switch(ch3)
                  {
                  case ' ':

                    if (ch4 == ' ')
                      return 3;

                    break;

                  case 'T':

                    if (ch4 == ' ')
                      return 5;

                    break;

                  case '1':

                    if (ch4 == 'P')
                      return 8;

                    break;

                  case '2':

                    if (ch4 == 'P')
                      return 9;
                    else if (ch4 == '*')
                      return 17;
                    else if (ch4 == ' ')
                      return 26;

                    break;

                  case '3':

                    if (ch4 == '*')
                      return 15;

                    break;

                  case '4':

                    if (ch4 == '*')
                      return 13;
                    else if (ch4 == ' ')
                      return 27;

                    break;

                  case '5':

                    if (ch4 == '*')
                      return 10;

                    break;

                  case '6':

                    if (ch4 == ' ')
                      return 28;

                    break;
                  }

                break;

              case 'P':

                if ((ch3 == ' ') && (ch4 == ' '))
                  return 7;

                break;

              case 'S':

                if (ch4 == ' ')
                  {
                    if (ch3 == ' ')
                      return 6;
                    else if (ch3 == 'G')
                      return 20;
                  }

                break;
              }
          }

        unsigned int refno;
        for( refno = MINELEM ; refno < ElemNo ; refno++ )
          if( !strncmp(ElemDesc[refno], atomid, 4) )
            return refno;

        if ( ElemNo < MAXELEM - 1 )
          {
            ElemNo++;
            ElemDesc[refno][0] = (char) ch1;
            ElemDesc[refno][1] = (char) ch2;
            ElemDesc[refno][2] = (char) ch3;
            ElemDesc[refno][3] = (char) ch4;
            return refno;
          }
        else
          {
            obErrorLog.ThrowError(__FUNCTION__, "Maximum number of atom ids exceeded", obWarning);
            return 0;
          }
      }
    else
      {
        obErrorLog.ThrowError(__FUNCTION__, "NULL Atom IDs specified", obWarning);
        return 0;
      }
  }

  static unsigned int GetResidueNumber(const char *res)
  {
    if (res != NULL)
      {
        int ch1 = toupper(res[0]);
        int ch2 = toupper(res[1]);
        int ch3 = toupper(res[2]);

        switch( ch1 )
          {
          case(' '):

            if( ch2 == ' ' )
              {
                switch( ch3 )
                  {
                  case('A'):  return( 24 );
                  case('C'):  return( 25 );
                  case('G'):  return( 26 );
                  case('T'):  return( 27 );
                  case('U'):  return( 28 );
                  case('I'):  return( 30 );
                  }
              }
            else if( ch2 == '+' )
              {
                if( ch3 == 'U' )
                  return( 29 );
              }
            else if( ch2 == 'Y' )
              {
                if( ch3 == 'G' )
                  return( 39 );
              }

            break;

          case('0'):

            if( ch2 == 'M' )
              {
                if( ch3 == 'C' )
                  return( 33 );
                else if( ch3 == 'G' )
                  return( 38 );
              }

            break;

          case('1'):

            if( ch2 == 'M' )
              {
                if( ch3 == 'A' )
                  return( 31 );
                else if( ch3 == 'G' )
                  return( 34 );
              }

            break;

          case('2'):

            if( ch2 == 'M' )
              if( ch3 == 'G' )
                return( 35 );

            break;

          case('5'):

            if( ch2 == 'M' )
              {
                if( ch3 == 'C' )
                  return( 32 );
                else if( ch3 == 'U' )
                  return( 41 );
              }

            break;

          case('7'):

            if( ch2 == 'M' )
              if( ch3 == 'G' )
                return( 37 );

            break;

          case('A'):

            if( ch2 == 'L' )
              {
                if( ch3 == 'A' )
                  return(  0 );
              }
            else if( ch2 == 'S' )
              {
                if( ch3 == 'P' )
                  return(  7 );
                else if( ch3 == 'N' )
                  return(  9 );
                else if( ch3 == 'X' )
                  return( 20 );
              }
            else if( ch2 == 'R' )
              {
                if( ch3 == 'G' )
                  return( 12 );
              }
            else if( ch2 == 'C' )
              {
                if( ch3 == 'E' )
                  return( 44 );
              }
            else if( ch2 == 'D' )
              {
                if( ch3 == 'E' )
                  return( 24 );    /* "ADE" -> "  A" */
              }

            break;

          case('C'):

            if( ch2 == 'Y' )
              {
                if( ch3 == 'S' )
                  return( 17 );
                else if( ch3 == 'H' )
                  return( 17 );    /* "CYH" -> "CYS" */
                else if( ch3 == 'T' )
                  return( 25 );    /* "CYT" -> "  C" */
              }
            else if( ch2 == 'O' )
              {
                if( ch3 == 'A' )
                  return( 51 );
              }
            else if( ch2 == 'P' )
              {
                if( ch3 == 'R' )
                  return( 11 );    /* "CPR" -> "PRO" */
              }
            else if( ch2 == 'S' )
              {
                if( ch3 == 'H' )
                  return( 17 );    /* "CSH" -> "CYS" */
                else if( ch3 == 'M' )
                  return( 17 );    /* "CSM" -> "CYS" */
              }

            break;

          case('D'):

            if( ch2 == 'O' )
              {
                if( ch3 == 'D' )
                  return( 47 );
              }
            else if( ch2 == '2' )
              {
                if( ch3 == 'O' )
                  return( 47 );    /* "D2O" -> "DOD" */
              }

            break;

          case('F'):

            if( ch2 == 'O' )
              if( ch3 == 'R' )
                return( 45 );

            break;

          case('G'):

            if( ch2 == 'L' )
              {
                if( ch3 == 'Y' )
                  return(  1 );
                else if( ch3 == 'U' )
                  return( 10 );
                else if( ch3 == 'N' )
                  return( 14 );
                else if( ch3 == 'X' )
                  return( 21 );
              }
            else if( ch2 == 'U' )
              {
                if( ch3 == 'A' )
                  return( 26 );    /* "GUA" -> "  G" */
              }

            break;

          case('H'):

            if( ch2 == 'I' )
              {
                if( ch3 == 'S' )
                  return( 16 );
              }
            else if( ch2 == 'O' )
              {
                if( ch3 == 'H' )
                  return( 46 );
              }
            else if( ch2 == 'Y' )
              {
                if( ch3 == 'P' )
                  return( 23 );
              }
            else if( ch2 == '2' )
              {
                if( ch3 == 'O' )
                  return( 46 );    /* "H20" -> "HOH" */
                else if( ch3 == 'U' )
                  return( 40 );
              }

            break;

          case('I'):

            if( ch2 == 'L' )
              if( ch3 == 'E' )
                return(  8 );

            break;

          case('L'):

            if( ch2 == 'E' )
              {
                if( ch3 == 'U' )
                  return(  2 );
              }
            else if( ch2 == 'Y' )
              {
                if( ch3 == 'S' )
                  return(  6 );
              }

            break;

          case('M'):

            if( ch2 == 'E' )
              {
                if( ch3 == 'T' )
                  return( 18 );
              }
            else if( ch2 == '2' )
              {
                if( ch3 == 'G' )
                  return( 36 );
              }

            break;

          case('N'):

            if( ch2 == 'A' )
              {
                if( ch3 == 'D' )
                  return( 50 );
                else if( ch3 == 'P' )
                  return( 52 );
              }
            else if( ch2 == 'D' )
              {
                if( ch3 == 'P' )
                  return( 53 );
              }

            break;

          case('P'):

            if( ch2 == 'R' )
              {
                if( ch3 == 'O' )
                  return( 11 );
              }
            else if( ch2 == 'H' )
              {
                if( ch3 == 'E' )
                  return( 13 );
              }
            else if( ch2 == 'C' )
              {
                if( ch3 == 'A' )
                  return( 22 );
              }
            else if( ch2 == 'O' )
              {
                if( ch3 == '4' )
                  return( 49 );
              }
            else if( ch2 == 'S' )
              {
                if( ch3 == 'U' )
                  return( 42 );
              }

            break;

          case('S'):

            if( ch2 == 'E' )
              {
                if( ch3 == 'R' )
                  return(  3 );
              }
            else if( ch2 == 'O' )
              {
                if( ch3 == '4' )
                  return( 48 );
                else if( ch3 == 'L' )
                  return( 46 );    /* "SOL" -> "HOH" */
              }
            else if( ch2 == 'U' )
              {
                if( ch3 == 'L' )
                  return( 48 );    /* "SUL" -> "SO4" */
              }

            break;

          case('T'):

            if( ch2 == 'H' )
              {
                if( ch3 == 'R' )
                  return(  5 );
                else if( ch3 == 'Y' )
                  return( 27 );    /* "THY" -> "  T" */
              }
            else if( ch2 == 'Y' )
              {
                if( ch3 == 'R' )
                  return( 15 );
              }
            else if( ch2 == 'R' )
              {
                if( ch3 == 'P' )
                  return( 19 );
                else if( ch3 == 'Y' )
                  return( 19 );    /* "TRY" -> "TRP" */
              }
            else if( ch2 == 'I' )
              {
                if( ch3 == 'P' )
                  return( 46 );    /* "TIP" -> "HOH" */
              }

            break;

          case('U'):

            if( ch2 == 'N' )
              {
                if( ch3 == 'K' )
                  return( 43 );
              }
            else if( ch2 == 'R' )
              {
                if( ch3 == 'A' )
                  return( 28 );    /* "URA" -> "  U" */
                else if( ch3 == 'I' )
                  return( 28 );    /* "URI" -> "  U" */
              }

            break;

          case('V'):

            if( ch2 == 'A' )
              if( ch3 == 'L' )
                return(  4 );

            break;

          case('W'):

            if( ch2 == 'A' )
              if( ch3 == 'T' )
                return( 46 );    /* "WAT" -> "HOH" */

            break;
          }

        unsigned int refno;
        for( refno = MINRES; refno < ResNo ; refno++ )
          if( !strncmp(Residue[refno],res,3) )
            return refno;

        if ( ResNo < MAXRES - 1 )
          {
            ResNo++;
            Residue[refno][0] = (char) ch1;
            Residue[refno][1] = (char) ch2;
            Residue[refno][2] = (char) ch3;
            return refno;
          }
      }

    return OBResidueIndex::UNK;
  }

  static void SetResidueKeys(const char   *residue,
                             unsigned int &reskey,
                             unsigned int &aakey)
  {
    reskey = GetResidueNumber(residue);
    switch(reskey)
      {
      case OBResidueIndex::ALA:
        aakey = AA_ALA;
        break;
      case OBResidueIndex::GLY:
        aakey = AA_GLY;
        break;
      case OBResidueIndex::LEU:
        aakey = AA_LEU;
        break;
      case OBResidueIndex::SER:
        aakey = AA_SER;
        break;
      case OBResidueIndex::VAL:
        aakey = AA_VAL;
        break;
      case OBResidueIndex::THR:
        aakey = AA_THR;
        break;
      case OBResidueIndex::LYS:
        aakey = AA_LYS;
        break;
      case OBResidueIndex::ASP:
        aakey = AA_ASP;
        break;
      case OBResidueIndex::ILE:
        aakey = AA_ILE;
        break;
      case OBResidueIndex::ASN:
        aakey = AA_ASN;
        break;
      case OBResidueIndex::GLU:
        aakey = AA_GLU;
        break;
      case OBResidueIndex::PRO:
        aakey = AA_PRO;
        break;
      case OBResidueIndex::ARG:
        aakey = AA_ARG;
        break;
      case OBResidueIndex::PHE:
        aakey = AA_PHE;
        break;
      case OBResidueIndex::GLN:
        aakey = AA_GLN;
        break;
      case OBResidueIndex::TYR:
        aakey = AA_TYR;
        break;
      case OBResidueIndex::HIS:
        aakey = AA_HIS;
        break;
      case OBResidueIndex::CYS:
        aakey = AA_CYS;
        break;
      case OBResidueIndex::MET:
        aakey = AA_MET;
        break;
      case OBResidueIndex::TRP:
        aakey = AA_TRP;
        break;
      default:
        aakey = 0;
        break;
      }
  }

  ///////////////////////////////////////////////////////////////////////////////
  // OBResidue: Constructors / Destructor
  ///////////////////////////////////////////////////////////////////////////////

  OBResidue::OBResidue()
  {
    _chain    = 'A';
    _idx      = 0;
    _aakey    = 0;
    _reskey   = OBResidueIndex::UNK;
    _resnum   = 0;
    _resname  = "";
    _vdata.clear();

  }

  OBResidue::OBResidue(const OBResidue &src) :
    OBBase()
  {
    _chain    = src._chain;
    _aakey    = src._aakey;
    _reskey   = src._reskey;
    _resnum   = src._resnum;
    _resname  = src._resname;
    _atomid   = src._atomid;
    _hetatm   = src._hetatm;
    _sernum   = src._sernum;

  }

  OBResidue::~OBResidue()
  {
    vector<OBAtom*>::iterator a;
    for ( a = _atoms.begin() ; a != _atoms.end() ; a++ )
      (*a)->SetResidue(NULL);
    _atoms.clear();

  }

  ///////////////////////////////////////////////////////////////////////////////
  // OBResidue: Operator Overloads
  ///////////////////////////////////////////////////////////////////////////////

  OBResidue &OBResidue::operator = (const OBResidue &src)
    //copy residue information
    // Currently does not copy vdata informtion
  {
    if (this != &src)
      {
        _chain    = src._chain;
        _aakey    = src._aakey;
        _reskey   = src._reskey;
        _resnum   = src._resnum;
        _resname  = src._resname;
        _atomid   = src._atomid;
        _hetatm   = src._hetatm;
        _sernum   = src._sernum;
      }

    return(*this);
  }

  ///////////////////////////////////////////////////////////////////////////////
  // OBResidue: Data Access / Manipulation
  ///////////////////////////////////////////////////////////////////////////////

  void OBResidue::AddAtom(OBAtom *atom)
  {
    if (atom != NULL)
      {
        atom->SetResidue(this);

        _atoms.push_back(atom);
        _atomid.push_back("");
        _hetatm.push_back(false);
        _sernum.push_back(0);
      }
  }

  void OBResidue::InsertAtom(OBAtom *atom)
  {
    if (atom != NULL)
      {
        atom->SetResidue(this);

        _atoms.push_back(atom);
        _atomid.push_back("");
        _hetatm.push_back(false);
        _sernum.push_back(0);
      }
  }

  void OBResidue::RemoveAtom(OBAtom *atom)
  {
    if (atom != NULL && _atoms.size())
      {
        for ( unsigned int i = 0 ; i < _atoms.size() ; i++ )
          {
            if (_atoms[i] != NULL && _atoms[i] == atom)
              {
                atom->SetResidue(NULL);
                _atoms.erase(_atoms.begin() + i);
                _atomid.erase(_atomid.begin() + i);
                _hetatm.erase(_hetatm.begin() + i);
                _sernum.erase(_sernum.begin() + i);
              }
          }
      }
  }

  void OBResidue::Clear(void)
  {
    for (unsigned int i = 0 ; i < _atoms.size() ; i++ )
      _atoms[i]->SetResidue(NULL);

    _chain   = 'A';
    _idx     = 0;
    _aakey   = 0;
    _reskey  = OBResidueIndex::UNK;
    _resnum  = 0;
    _resname = "";

    _atoms.clear();
    _atomid.clear();
    _hetatm.clear();
    _sernum.clear();
  }

  void OBResidue::SetChain(char chain)
  {
    _chain = chain;
  }

  void OBResidue::SetChainNum(unsigned int chainnum)
  {
    _chain = (char) ('A' + chainnum - 1);
  }

  void OBResidue::SetIdx(unsigned int idx)
  {
    _idx = idx;
  }

  void OBResidue::SetName(const string &name)
  {
    _resname = name;
    SetResidueKeys(_resname.c_str(), _reskey, _aakey);
  }

  void OBResidue::SetNum(unsigned int resnum)
  {
    _resnum = resnum;
  }

  void OBResidue::SetAtomID(OBAtom *atom, const string &id)
  {
    for ( unsigned int i = 0 ; i < _atoms.size() ; i++ )
      if (_atoms[i] == atom)
        _atomid[i] = id;
  }

  void OBResidue::SetHetAtom(OBAtom *atom, bool hetatm)
  {
    for ( unsigned int i = 0 ; i < _atoms.size() ; i++ )
      if (_atoms[i] == atom)
        _hetatm[i] = hetatm;
  }

  void OBResidue::SetSerialNum(OBAtom *atom, unsigned int sernum)
  {
    for ( unsigned int i = 0 ; i < _atoms.size() ; i++ )
      if (_atoms[i] == atom)
        _sernum[i] = sernum;
  }

  vector<OBAtom*> OBResidue::GetAtoms(void) const
  {
    return _atoms;
  }

  vector<OBBond*> OBResidue::GetBonds(bool exterior) const
  {
    OBAtom         *atom;
    vector<OBBond*> bonds;
    OBBitVec        idxs;
    unsigned int    sz;

    sz = (unsigned int) _atoms.size();
    for ( unsigned int i = 0 ; i < sz ; i++ )
      {
        atom = _atoms[i];
        OBBond *bond;
        vector<OBEdgeBase*>::iterator b;
        for (bond = atom->BeginBond(b) ; bond ; bond = atom->NextBond(b))
          {
            if (!idxs.BitIsOn(bond->GetIdx()))
              {
                if (!exterior)
                  {
                    if (bond->GetNbrAtom(atom)->GetResidue() == this)
                      bonds.push_back(&(*bond));
                  }
                else
                  bonds.push_back(&(*bond));

                idxs.SetBitOn(bond->GetIdx());
              }
          }
      }

    return bonds;
  }

  string OBResidue::GetName(void) const
  {
    return _resname;
  }

  unsigned int OBResidue::GetNum(void) const
  {
    return _resnum;
  }

  unsigned int OBResidue::GetNumAtoms(void) const
  {
    return (unsigned int)_atoms.size();
  }

  char OBResidue::GetChain(void) const
  {
    return _chain;
  }

  unsigned int OBResidue::GetChainNum(void) const
  {
    if (isdigit(_chain))
      return (_chain - '0');
    else
      return (_chain - 'A' + 1);
  }

  unsigned int OBResidue::GetIdx(void) const
  {
    return(_idx);
  }

  unsigned int OBResidue::GetResKey(void) const
  {
    return(_reskey);
  }

  string OBResidue::GetAtomID(OBAtom *atom) const
  {
    for ( unsigned int i = 0 ; i < _atoms.size() ; i++ )
      if (_atoms[i] == atom)
        return _atomid[i];
    return "";
  }

  unsigned int OBResidue::GetSerialNum(OBAtom *atom) const
  {
    for ( unsigned int i = 0 ; i < _atoms.size() ; i++ )
      if (_atoms[i] == atom)
        return _sernum[i];
    return 0;
  }

  bool OBResidue::IsHetAtom(OBAtom *atom) const
  {
    for ( unsigned int i = 0 ; i < _atoms.size() ; i++ )
      if (_atoms[i] == atom)
        return _hetatm[i];
    return false;
  }

  ///////////////////////////////////////////////////////////////////////////////
  // OBResidue: Iteration Utilities
  ///////////////////////////////////////////////////////////////////////////////

  OBAtom *OBResidue::BeginAtom(vector<OBAtom*>::iterator &i)
  {
    i = _atoms.begin();
    return ((i == _atoms.end()) ? NULL : *i);
  }

  OBAtom *OBResidue::NextAtom(vector<OBAtom*>::iterator &i)
  {
    i++;
    return ((i == _atoms.end()) ? NULL : *i);
  }

  ///////////////////////////////////////////////////////////////////////////////
  // OBResidue: Information Functions
  ///////////////////////////////////////////////////////////////////////////////

  bool OBResidue::GetAminoAcidProperty(int property) const
  {
    switch(property)
      {
      case OBAminoAcidProperty::ACIDIC:
        return IS_ACIDIC(_aakey)      != 0;
      case OBAminoAcidProperty::ACYCLIC:
        return IS_ACYCLIC(_aakey)     != 0;
      case OBAminoAcidProperty::ALIPHATIC:
        return IS_ALIPHATIC(_aakey)   != 0;
      case OBAminoAcidProperty::AROMATIC:
        return IS_AROMATIC(_aakey)    != 0;
      case OBAminoAcidProperty::BASIC:
        return IS_BASIC(_aakey)       != 0;
      case OBAminoAcidProperty::BURIED:
        return IS_BURIED(_aakey)      != 0;
      case OBAminoAcidProperty::CHARGED:
        return IS_CHARGED(_aakey)     != 0;
      case OBAminoAcidProperty::CYCLIC:
        return IS_CYCLIC(_aakey)      != 0;
      case OBAminoAcidProperty::HYDROPHOBIC:
        return IS_HYDROPHOBIC(_aakey) != 0;
      case OBAminoAcidProperty::LARGE:
        return IS_LARGE(_aakey)       != 0;
      case OBAminoAcidProperty::MEDIUM:
        return IS_MEDIUM(_aakey)      != 0;
      case OBAminoAcidProperty::NEGATIVE:
        return IS_NEGATIVE(_aakey)    != 0;
      case OBAminoAcidProperty::NEUTRAL:
        return IS_NEUTRAL(_aakey)     != 0;
      case OBAminoAcidProperty::POLAR:
        return IS_POLAR(_aakey)       != 0;
      case OBAminoAcidProperty::POSITIVE:
        return IS_POSITIVE(_aakey)    != 0;
      case OBAminoAcidProperty::SMALL:
        return IS_SMALL(_aakey)       != 0;
      case OBAminoAcidProperty::SURFACE:
        return IS_SURFACE(_aakey)     != 0;
      default:
        return false;
      }
  }

  bool OBResidue::GetAtomProperty(OBAtom *atom, int property) const
  {
    if (atom != NULL)
      {
        unsigned int atomid = GetAtomIDNumber(GetAtomID(atom).c_str());

        switch(property)
          {
          case OBResidueAtomProperty::ALPHA_CARBON:
            return (atomid == 1);

          case OBResidueAtomProperty::AMINO_BACKBONE:
            return (atomid <= 3);

          case OBResidueAtomProperty::BACKBONE:
            return (atomid <= 18);

          case OBResidueAtomProperty::CYSTEINE_SULPHUR:
            return (atomid == 20);

          case OBResidueAtomProperty::LIGAND:
            return IsHetAtom(atom) &&
              !GetResidueProperty(OBResidueProperty::SOLVENT);

          case OBResidueAtomProperty::NUCLEIC_BACKBONE:
            return ((atomid >= 7) && (atomid <= 18));

          case OBResidueAtomProperty::SHAPELY_BACKBONE:
            return (atomid <= 7);

          case OBResidueAtomProperty::SHAPELY_SPECIAL:
            return (atomid == 19);

          case OBResidueAtomProperty::SIDECHAIN:
            return GetResidueProperty(OBResidueProperty::AMINO_NUCLEO) &&
              (atomid > 18);

          case OBResidueAtomProperty::SUGAR_PHOSPHATE:
            return (atomid == 7);
          }
      }

    return false;
  }

  bool OBResidue::GetResidueProperty(int property) const
  {
    switch(property)
      {
      case OBResidueProperty::AMINO:
        return (_reskey <= OBResidueIndex::HYP);

      case OBResidueProperty::AMINO_NUCLEO:
        return (_reskey <= OBResidueIndex::PSU);

      case OBResidueProperty::COENZYME:
        return (_reskey >= OBResidueIndex::NAD) &&
          (_reskey <= OBResidueIndex::NDP);

      case OBResidueProperty::ION:
        return (_reskey == OBResidueIndex::SO4) ||
          (_reskey == OBResidueIndex::PO4);

      case OBResidueProperty::NUCLEO:
        return (_reskey >= OBResidueIndex::A) &&
          (_reskey <= OBResidueIndex::PSU);

      case OBResidueProperty::PROTEIN:
        return (_reskey <= OBResidueIndex::HYP) ||
          ((_reskey >= OBResidueIndex::UNK) &&
           (_reskey <= OBResidueIndex::FOR));

      case OBResidueProperty::PURINE:
        return (_reskey == OBResidueIndex::A) ||
          (_reskey == OBResidueIndex::G);

      case OBResidueProperty::PYRIMIDINE:
        return (_reskey == OBResidueIndex::C) ||
          (_reskey == OBResidueIndex::T);

      case OBResidueProperty::SOLVENT:
        return (_reskey >= OBResidueIndex::HOH) &&
          (_reskey <= OBResidueIndex::PO4);

      case OBResidueProperty::WATER:
        return (_reskey == OBResidueIndex::HOH) ||
          (_reskey == OBResidueIndex::DOD);

      default:
        return false;
      }
  }

  bool OBResidue::IsResidueType(int restype) const
  {
    return ((int)_reskey == restype);
  }

  /*Now in OBBase
  // OBGenericData methods
  bool OBResidue::HasData(string &s)
  //returns true if the generic attribute/value pair exists
  {
  if (_vdata.empty())
  return(false);

  vector<OBGenericData*>::iterator i;

  for (i = _vdata.begin();i != _vdata.end();i++)
  if ((*i)->GetAttribute() == s)
  return(true);

  return(false);
  }

  bool OBResidue::HasData(const char *s)
  //returns true if the generic attribute/value pair exists
  {
  if (_vdata.empty())
  return(false);

  vector<OBGenericData*>::iterator i;

  for (i = _vdata.begin();i != _vdata.end();i++)
  if ((*i)->GetAttribute() == s)
  return(true);

  return(false);
  }

  bool OBResidue::HasData(unsigned int dt)
  //returns true if the generic attribute/value pair exists
  {
  if (_vdata.empty())
  return(false);

  vector<OBGenericData*>::iterator i;

  for (i = _vdata.begin();i != _vdata.end();i++)
  if ((*i)->GetDataType() == dt)
  return(true);

  return(false);
  }

  OBGenericData *OBResidue::GetData(string &s)
  //returns the value given an attribute
  {
  vector<OBGenericData*>::iterator i;

  for (i = _vdata.begin();i != _vdata.end();i++)
  if ((*i)->GetAttribute() == s)
  return(*i);

  return(NULL);
  }

  OBGenericData *OBResidue::GetData(const char *s)
  //returns the value given an attribute
  {
  vector<OBGenericData*>::iterator i;

  for (i = _vdata.begin();i != _vdata.end();i++)
  if ((*i)->GetAttribute() == s)
  return(*i);

  return(NULL);
  }

  OBGenericData *OBResidue::GetData(unsigned int dt)
  {
  vector<OBGenericData*>::iterator i;
  for (i = _vdata.begin();i != _vdata.end();i++)
  if ((*i)->GetDataType() == dt)
  return(*i);
  return(NULL);
  }

  void OBResidue::DeleteData(unsigned int dt)
  {
  vector<OBGenericData*> vdata;
  vector<OBGenericData*>::iterator i;
  for (i = _vdata.begin();i != _vdata.end();i++)
  if ((*i)->GetDataType() == dt)
  delete *i;
  else
  vdata.push_back(*i);
  _vdata = vdata;
  }

  void OBResidue::DeleteData(vector<OBGenericData*> &vg)
  {
  vector<OBGenericData*> vdata;
  vector<OBGenericData*>::iterator i,j;

  bool del;
  for (i = _vdata.begin();i != _vdata.end();i++)
  {
  del = false;
  for (j = vg.begin();j != vg.end();j++)
  if (*i == *j)
  {
  del = true;
  break;
  }
  if (del)
  delete *i;
  else
  vdata.push_back(*i);
  }
  _vdata = vdata;
  }

  void OBResidue::DeleteData(OBGenericData *gd)
  {
  vector<OBGenericData*>::iterator i;
  for (i = _vdata.begin();i != _vdata.end();i++)
  if (*i == gd)
  {
  delete *i;
  _vdata.erase(i);
  }

  }
  */

} // end namespace OpenBabel

//! \file residue.cpp
//! \brief Handle macromolecule residues.
