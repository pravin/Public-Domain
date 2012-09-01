#!/usr/bin/env python

from AddressBook import ABAddressBook, kABFirstNameProperty, kABLastNameProperty, kABEmailProperty
import re

class ExportForPine:
    @staticmethod
    def fetch():
        ab = ABAddressBook.sharedAddressBook()
        for person in ab.people():
            first_name = person.valueForProperty_(kABFirstNameProperty)
            if first_name == None: first_name = ''

            last_name = person.valueForProperty_(kABLastNameProperty)
            if last_name == None: last_name = ''

            emails = person.valueForProperty_(kABEmailProperty)
            if emails:
                primaryid = emails.primaryIdentifier()
                email = emails.valueAtIndex_(emails.indexForIdentifier_(primaryid))
                
                fields = []
                fields.append(re.sub('[^A-Za-z0-9]', '', first_name.lower() + last_name.lower()))
                fields.append("%s, %s" % (last_name, first_name))
                fields.append("%s %s <%s>" % (first_name, last_name, email))

                print "\t".join(fields)

if __name__ == '__main__':
    ExportForPine.fetch()
