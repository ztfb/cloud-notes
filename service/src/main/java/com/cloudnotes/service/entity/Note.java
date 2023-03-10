package com.cloudnotes.service.entity;

import lombok.Data;

import javax.persistence.*;
import java.util.Date;

@Entity
@Data
@Table(name = "note")
public class Note {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private int id;
    @Column(name = "name")
    private String name;
    @Column(name = "author")
    private String author;
    @Column(name = "label")
    private String label;
    @Column(name = "share_code")
    private String shareCode;
    @Column(name = "default_power")
    private int defaultPower;
    @Column(name = "user_id")
    private int userId;
    @Column(name = "catalog_id")
    private int catalogId;
}
