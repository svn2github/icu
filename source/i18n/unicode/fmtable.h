/*
********************************************************************************
*   Copyright (C) 1997-2003, International Business Machines
*   Corporation and others.  All Rights Reserved.
********************************************************************************
*
* File FMTABLE.H
*
* Modification History:
*
*   Date        Name        Description
*   02/29/97    aliu        Creation.
********************************************************************************
*/
#ifndef FMTABLE_H
#define FMTABLE_H

#include "unicode/utypes.h"
#include "unicode/unistr.h"

#if !UCONFIG_NO_FORMATTING

U_NAMESPACE_BEGIN

/**
 * Formattable objects can be passed to the Format class or
 * its subclasses for formatting.  Formattable is a thin wrapper
 * class which interconverts between the primitive numeric types
 * (double, long, etc.) as well as UDate and UnicodeString.
 * <P>
 * Note that this is fundamentally different from the Java behavior, since
 * in this case the various formattable objects do not occupy a hierarchy,
 * but are all wrapped within this one class.  Formattable encapsulates all
 * the polymorphism in itself.
 * <P>
 * It would be easy to change this so that Formattable was an abstract base
 * class of a genuine hierarchy, and that would clean up the code that
 * currently must explicitly check for type, but that seems like overkill at
 * this point.
 *
 * The Formattable class is not suitable for subclassing.
 */
class U_I18N_API Formattable : public UObject {
public:
    /**
     * This enum is only used to let callers distinguish between
     * the Formattable(UDate) constructor and the Formattable(double)
     * constructor; the compiler cannot distinguish the signatures,
     * since UDate is currently typedefed to be either double or long.
     * If UDate is changed later to be a bonafide class
     * or struct, then we no longer need this enum.
     * @stable ICU 2.4
     */
    enum ISDATE { kIsDate };

    /**
     * Default constructor
     * @stable ICU 2.4
     */
    Formattable(); // Type kLong, value 0

    /**
     * Creates a Formattable object with a UDate instance.
     * @param d the UDate instance.
     * @param flag the flag to indicate this is a date. Always set it to kIsDate
     * @stable ICU 2.0  
     */
    Formattable(UDate d, ISDATE flag);

    /**
     * Creates a Formattable object with a double number.
     * @param d the double number.
     * @stable ICU 2.0
     */
    Formattable(double d);

    /**
     * Creates a Formattable object with a long number.
     * @param l the long number.
     * @stable ICU 2.0
     */
    Formattable(int32_t l);

    /**
     * Creates a Formattable object with an int64_t number
     * @param ll the int64_t number.
     * @draft ICU 2.8
     */
    Formattable(int64_t ll);

    /**
     * Creates a Formattable object with a char string pointer.
     * Assumes that the char string is null terminated.
     * @param strToCopy the char string.
     * @stable ICU 2.0
     */
    Formattable(const char* strToCopy);

    /**
     * Creates a Formattable object with a UnicodeString object to copy from.
     * @param strToCopy the UnicodeString string.
     * @stable ICU 2.0
     */
    Formattable(const UnicodeString& strToCopy);

    /**
     * Creates a Formattable object with a UnicodeString object to adopt from.
     * @param strToAdopt the UnicodeString string.
     * @stable ICU 2.0
     */
    Formattable(UnicodeString* strToAdopt);

    /**
     * Creates a Formattable object with an array of Formattable objects.
     * @param arrayToCopy the Formattable object array.
     * @param count the array count.
     * @stable ICU 2.0
     */
    Formattable(const Formattable* arrayToCopy, int32_t count);

    /**
     * Copy constructor.
     * @stable ICU 2.0
     */
    Formattable(const Formattable&);

    /**
     * Assignment operator.
     * @param rhs   The Formattable object to copy into this object.
     * @stable ICU 2.0
     */
    Formattable&    operator=(const Formattable &rhs);

    /**
     * Equality comparison.
     * @param other    the object to be compared with.
     * @return        TRUE if other are equal to this, FALSE otherwise.
     * @stable ICU 2.0
     */
    UBool          operator==(const Formattable &other) const;
    
    /** 
     * Equality operator.
     * @param other    the object to be compared with.
     * @return        TRUE if other are unequal to this, FALSE otherwise.
     * @stable ICU 2.0
     */
    UBool          operator!=(const Formattable& other) const
      { return !operator==(other); }

    /** 
     * Destructor.
     * @stable ICU 2.0
     */
    virtual         ~Formattable();

    /**
     * Clone this object.
     * Clones can be used concurrently in multiple threads.
     * If an error occurs, then NULL is returned.
     * The caller must delete the clone.
     *
     * @return a clone of this object
     *
     * @see getDynamicClassID
     * @draft ICU 2.8
     */
    Formattable *clone() const;

    /** 
     * The list of possible data types of this Formattable object.
     * @stable ICU 2.4
     */
    enum Type {
                /** @stable ICU 2.4 */
        kDate,      // Date
                /** @stable ICU 2.4 */
        kDouble,    // double
                /** @stable ICU 2.4 */
        kLong,      // long
                /** @stable ICU 2.4 */
        kString,    // UnicodeString
                /** @stable ICU 2.4 */
        kArray,     // Formattable[]
                /** @draft ICU 2.8 */
                kInt64      // int64
   };

    /**
     * Gets the data type of this Formattable object.
     * @return    the data type of this Formattable object.
     * @stable ICU 2.0
     */
    Type            getType(void) const;
    
    /**
     * Gets the double value of this object.
     * @return    the double value of this object.
     * @stable ICU 2.0
     */ 
    double          getDouble(void) const { return fValue.fDouble; }

    /**
     * Gets the double value of this object.  This converts from long or
     * int64 values as required (conversion from int64 can lose precision).
     * If the type is not a numeric type, 0 is returned and the status
     * is set to U_INVALID_FORMAT_ERROR.
     * @param status the error code
     * @return    the double value of this object.
     * @draft ICU 2.8
     */ 
    double          getDouble(UErrorCode* status) const;

    /**
     * Gets the long value of this object.
     * @return    the long value of this object.
     * @stable ICU 2.0
     */ 
    int32_t         getLong(void) const { return (int32_t)fValue.fInt64; }

    /**
     * Gets the long value of this object.  This converts from double or
     * int64 values as required.  If the magnitude is too large to fit in a long,
     * the maximum or minimum long value, as appropriate, is returned and
     * the status is set to U_INVALID_FORMAT_ERROR.
     * If the type is not a numeric type, 0 is returned and the status 
     * is set to U_INVALID_FORMAT_ERROR.
     * @param status the error code
     * @return    the long value of this object.
     * @stable ICU 2.0
     */ 
    int32_t         getLong(UErrorCode* status) const;

    /**
     * Gets the int64 value of this object.
     * @return    the int64 value of this object.
     * @draft ICU 2.8
     */ 
    int64_t         getInt64(void) const { return fValue.fInt64; }

    /**
     * Gets the int64 value of this object.  This converts from double or
     * int64 values as required.  If the value value won't fit in an int64,
     * the maximum or minimum in64 value, as appropriate, is returned and
     * the status is set to U_INVALID_FORMAT_ERROR.
     * If the type is not a numeric type, 0 is returned and the status 
     * is set to U_INVALID_FORMAT_ERROR.
     * @param status the error code
     * @return    the int64 value of this object.
     * @draft ICU 2.8
     */ 
    int64_t         getInt64(UErrorCode* status) const;

    /**
     * Gets the Date value of this object.
     * @return    the Date value of this object.
     * @stable ICU 2.0
     */ 
    UDate           getDate() const { return fValue.fDate; }

    /**
     * Gets the Date value of this object.
     * @param status the error code.  If the type is not a date, status
     * is set to U_INVALID_FORMAT_ERROR and the return value is undefined.
     * @return    the Date value of this object.
     * @draft ICU 2.8
     */ 
     UDate          getDate(UErrorCode* status) const;

    /**
     * Gets the string value of this object.
     * @param result    Output param to receive the Date value of this object.
     * @return          A reference to 'result'.
     * @stable ICU 2.0
     */ 
    UnicodeString&  getString(UnicodeString& result) const
      { result=*fValue.fString; return result; }

    /**
     * Gets the string value of this object.
     * @param result    Output param to receive the Date value of this object.
     * @param status    the error code.  If the type is not a string, status
     * is set to U_INVALID_FORMAT_ERROR and the result is set to bogus.
     * @return          A reference to 'result'.
     * @draft ICU 2.8
     */ 
    UnicodeString&  getString(UnicodeString& result, UErrorCode* status) const;

    /**
     * Gets a const reference to the string value of this object.
     * @return   a const reference to the string value of this object.
     * @stable ICU 2.0
     */
    inline const UnicodeString& getString(void) const;

    /**
     * Gets a const reference to the string value of this object.
     * @param status    the error code.  If the type is not a string, status
     * is set to U_INVALID_FORMAT_ERROR and the result is a bogus string.
     * @return   a const reference to the string value of this object.
     * @draft ICU 2.8
     */
    const UnicodeString& getString(UErrorCode* status) const;

    /**
     * Gets a reference to the string value of this object.
     * @return   a reference to the string value of this object.
     * @stable ICU 2.0
     */
    inline UnicodeString& getString(void);

    /**
     * Gets a reference to the string value of this object.
     * @param status    the error code.  If the type is not a string, status
     * is set to U_INVALID_FORMAT_ERROR and the result is a bogus string.
     * @return   a reference to the string value of this object.
     * @draft ICU 2.8
     */
    UnicodeString& getString(UErrorCode* status);

    /**
     * Gets the array value and count of this object.
     * @param count    fill-in with the count of this object.
     * @return         the array value of this object.
     * @stable ICU 2.0
     */ 
    const Formattable* getArray(int32_t& count) const
      { count=fValue.fArrayAndCount.fCount; return fValue.fArrayAndCount.fArray; }

    /**
     * Gets the array value and count of this object.
     * @param count    fill-in with the count of this object.
     * @param status the error code.  If the type is not an array, status
     * is set to U_INVALID_FORMAT_ERROR, count is set to 0, and the result is NULL.
     * @return         the array value of this object.
     * @draft ICU 2.8
     */ 
    const Formattable* getArray(int32_t& count, UErrorCode* status) const;

    /**
     * Accesses the specified element in the array value of this Formattable object.
     * @param index the specified index.
     * @return the accessed element in the array.
     * @stable ICU 2.0
     */
    Formattable&    operator[](int32_t index) { return fValue.fArrayAndCount.fArray[index]; }

    /**
     * Sets the double value of this object.
     * @param d    the new double value to be set.
     * @stable ICU 2.0
     */ 
    void            setDouble(double d);

    /**
     * Sets the long value of this object.
     * @param l    the new long value to be set.
     * @stable ICU 2.0
     */ 
    void            setLong(int32_t l);

    /**
     * Sets the int64 value of this object.
     * @param ll    the new int64 value to be set.
     * @draft ICU 2.8
     */ 
    void            setInt64(int64_t ll);

    /**
     * Sets the Date value of this object.
     * @param d    the new Date value to be set.
     * @stable ICU 2.0
     */ 
    void            setDate(UDate d);

    /**
     * Sets the string value of this object.
     * @param stringToCopy    the new string value to be set.
     * @stable ICU 2.0
     */ 
    void            setString(const UnicodeString& stringToCopy);

    /**
     * Sets the array value and count of this object.
     * @param array    the array value.
     * @param count    the number of array elements to be copied.
     * @stable ICU 2.0
     */ 
    void            setArray(const Formattable* array, int32_t count);

    /**
     * Sets and adopts the string value and count of this object.
     * @param stringToAdopt    the new string value to be adopted.
     * @stable ICU 2.0
     */ 
    void            adoptString(UnicodeString* stringToAdopt);

    /**
     * Sets and adopts the array value and count of this object.
     * @stable ICU 2.0
     */ 
    void            adoptArray(Formattable* array, int32_t count);
        
    /**
     * ICU "poor man's RTTI", returns a UClassID for the actual class.
     *
     * @stable ICU 2.2
     */
    virtual UClassID getDynamicClassID() const;

    /**
     * ICU "poor man's RTTI", returns a UClassID for this class.
     *
     * @stable ICU 2.2
     */
    static UClassID getStaticClassID();

private:
    /**
     * Cleans up the memory for unwanted values.  For example, the adopted
     * string or array objects.
     */
    void            dispose(void);

    /**
     * Creates a new Formattable array and copies the values from the specified
     * original.
     * @param array the original array
     * @param count the original array count
     * @return the new Formattable array.
     */
    static Formattable* createArrayCopy(const Formattable* array, int32_t count);

    UnicodeString* getBogus() const;

    // Note: For now, we do not handle unsigned long and unsigned
    // double types.  Smaller unsigned types, such as unsigned
    // short, can fit within a long.
    union {
        UnicodeString*  fString;
        double          fDouble;
        int64_t         fInt64;
        UDate           fDate;
        struct
        {
            Formattable*  fArray;
            int32_t       fCount;
        }               fArrayAndCount;
    }                   fValue;

    Type                fType;
    UnicodeString       fBogus; // Bogus string when it's needed.
};

inline Formattable*
Formattable::createArrayCopy(const Formattable* array, int32_t count)
{
    Formattable *result = new Formattable[count];
    for (int32_t i=0; i<count; ++i) result[i] = array[i]; // Don't memcpy!
    return result;
}

inline UDate Formattable::getDate(UErrorCode* status) const {
    if (status && U_SUCCESS(*status) && fType != kDate) {
      *status = U_INVALID_FORMAT_ERROR;
      return 0;
    }
    return fValue.fDate;
}

inline const UnicodeString& Formattable::getString(void) const {
    return *fValue.fString;
}

inline UnicodeString& Formattable::getString(void) {
    return *fValue.fString;
}

U_NAMESPACE_END

#endif /* #if !UCONFIG_NO_FORMATTING */

#endif //_FMTABLE
//eof
     
